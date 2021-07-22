/**
 *  @note This file is part of MABE, https://github.com/mercere99/MABE2
 *  @copyright Copyright (C) Michigan State University, MIT Software license; see doc/LICENSE.md
 *  @date 2019-2021.
 * 
 *  @file  MABE.cpp
 *  @brief Tests for the main MABE object 
 */
#include <functional>
// CATCH
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
// MABE
#include "core/TraitManager.hpp"
#include "core/MABE.hpp"
#include "evaluate/static/EvalNK.hpp"

// Even if an error is thrown, the traitmap still updates with the new trait!

TEST_CASE("TraitInfo_Locks", "[core]"){
  { 
    //  [SETUP]
    // Create the TraitInfo to be tested (TraitInfo is abstract so we must make a TypedTraitInfo)
    auto error_func = [](const std::string & s){
      std::cout << "Error: " << s;
    }; 
    auto warning_func = [](const std::string & s){
      std::cout << "Warning: " << s;
    }; 
    mabe::ErrorManager error_man(error_func, warning_func);
    mabe::TraitManager<mabe::ModuleBase> trait_man(error_man);

    //  [BEGIN TESTS]
    // Make sure GetLocked returns a boolean
    REQUIRE(trait_man.GetLocked() <= 1); 
    REQUIRE(trait_man.GetLocked() >= 0);
    
    // Manager should begin as locked 
    REQUIRE(trait_man.GetLocked()); 

    // Check that Unlock works
    trait_man.Unlock(); 
    REQUIRE_FALSE(trait_man.GetLocked()); 

    // Check that Lock works
    trait_man.Lock(); 
    REQUIRE(trait_man.GetLocked()); 
  } 
}

TEST_CASE("TraitInfo_Basic", "[core]"){
  { 
    //  [SETUP]
    // Add module(s) to access the trait
    mabe::MABE control(0, NULL);
    control.AddPopulation("test_pop");
    mabe::EvalNK nk_mod(control);
    mabe::EvalNK nk2_mod(control); 

    // Setup a TraitManager
    // Use bools to tell if an error has been thrown 
    bool has_error_been_thrown = false; 
    bool has_warning_been_thrown = false; 

    auto error_func = [&has_error_been_thrown](const std::string & s){
      std::cout << "Error: " << s;
      has_error_been_thrown = true;
    }; 
    auto warning_func = [&has_warning_been_thrown](const std::string & s){
      std::cout << "Warning: " << s;
      has_warning_been_thrown = true; 
    }; 
    
    mabe::ErrorManager error_man(error_func, warning_func);
    error_man.Activate(); 
    mabe::TraitManager<mabe::ModuleBase> trait_man(error_man);

    //  [BEGIN TESTS]
    // Check that traitmap begins as empty
    REQUIRE(trait_man.GetSize() == 0); 

    // Check manager is initially locked
    REQUIRE(trait_man.GetLocked()); 

    // Traits cannot be added if manager is locked
    // Verified due to error being thrown 
    trait_man.AddTrait(&nk_mod, mabe::TraitInfo::Access::OPTIONAL, "trait_i", "a trait", emp::GetTypeID<double>()); 
    REQUIRE(has_error_been_thrown); 
    REQUIRE_FALSE(has_warning_been_thrown); 
    REQUIRE(trait_man.GetLocked()); 
    REQUIRE(trait_man.GetSize() == 1); 

    // Reset error flag and unlock manager
    has_error_been_thrown = false; 
    trait_man.Unlock(); 

    // Check trait with unknown access should throw error
    trait_man.AddTrait(&nk_mod, mabe::TraitInfo::Access::UNKNOWN, "trait_k", "a trait", emp::GetTypeID<int>()); 
    REQUIRE(has_error_been_thrown); 
    REQUIRE_FALSE(has_warning_been_thrown); 
    REQUIRE(trait_man.GetSize() == 2); 

    // Reset error flag
    has_error_been_thrown = false; 

    // Add a trait regularly and check to see if traitmap expands
    trait_man.AddTrait(&nk_mod, mabe::TraitInfo::Access::OPTIONAL, "trait_l", "a trait", emp::GetTypeID<int>()); 
    REQUIRE_FALSE(has_error_been_thrown); 
    REQUIRE_FALSE(has_warning_been_thrown); 
    REQUIRE(trait_man.GetSize() == 3); 

    // Add same trait to a different module
    // Shouldn't expand traitmap
    trait_man.AddTrait(&nk2_mod, mabe::TraitInfo::Access::OPTIONAL, "trait_l", "a trait", emp::GetTypeID<int>()); 
    REQUIRE_FALSE(has_error_been_thrown); 
    REQUIRE_FALSE(has_warning_been_thrown); 
    REQUIRE(trait_man.GetSize() == 3); 
  } 
}

TEST_CASE("TraitInfo_AddTrait", "[core]"){
  { 
    //  [SETUP]
    // Create the TraitInfo to be tested (TraitInfo is abstract so we must make a TypedTraitInfo)
    // Add module(s) to access the trait
    mabe::MABE control(0, NULL);
    control.AddPopulation("test_pop");
    mabe::EvalNK nk_mod(control);
    mabe::EvalNK nk2_mod(control); 

    // Setup a TraitManager
    // Use bools to tell if an error has been thrown 
    bool has_error_been_thrown = false; 
    bool has_warning_been_thrown = false; 

    auto error_func = [&has_error_been_thrown](const std::string & s){
      std::cout << "Error: " << s;
      has_error_been_thrown = true;
    }; 
    auto warning_func = [&has_warning_been_thrown](const std::string & s){
      std::cout << "Warning: " << s;
      has_warning_been_thrown = true; 
    }; 
    mabe::ErrorManager error_man(error_func, warning_func);
    error_man.Activate(); 
    mabe::TraitManager<mabe::ModuleBase> trait_man(error_man);
    trait_man.Unlock(); 

    //  [BEGIN TESTS]
    // Initial traitmap is empty
    REQUIRE(trait_man.GetSize() == 0); 

    // Add a trait normally
    trait_man.AddTrait(&nk_mod, mabe::TraitInfo::Access::OPTIONAL, "trait_i", "a trait", emp::GetTypeID<std::string>()); 
    REQUIRE_FALSE(has_error_been_thrown); 
    REQUIRE_FALSE(has_warning_been_thrown); 
    REQUIRE(trait_man.GetSize() == 1); 

    // Add same trait to same module
    // Should throw error and not add it again to the map
    trait_man.AddTrait(&nk_mod, mabe::TraitInfo::Access::OPTIONAL, "trait_i", "a trait", emp::GetTypeID<std::string>()); 
    REQUIRE(has_error_been_thrown); 
    REQUIRE_FALSE(has_warning_been_thrown); 
    REQUIRE(trait_man.GetSize() == 1);

    has_error_been_thrown = false; 

    // Add the same trait, but with a non-AltType 
    // Should throw error, and not add to traitmap <<- this fails??
    // trait_man.AddTrait(&nk2_mod, mabe::TraitInfo::Access::OPTIONAL, "trait_i", "a trait", emp::GetTypeID<int>()); 
    // REQUIRE(has_error_been_thrown); 
    // REQUIRE_FALSE(has_warning_been_thrown); 
    // REQUIRE(trait_man.GetSize() == 1); 

    has_error_been_thrown = false; 

    // Add a trait with specified optional types
    trait_man.AddTrait<int, double, float>(&nk_mod, mabe::TraitInfo::Access::OPTIONAL, "trait_j", "a trait", emp::GetTypeID<int>()); 
    REQUIRE_FALSE(has_error_been_thrown); 
    REQUIRE_FALSE(has_warning_been_thrown); 
    REQUIRE(trait_man.GetSize() == 2); 

    // Add it to another module using one of the specified alt-types
    trait_man.AddTrait(&nk2_mod, mabe::TraitInfo::Access::OPTIONAL, "trait_j", "a trait", emp::GetTypeID<float>()); 
    REQUIRE_FALSE(has_error_been_thrown); 
    REQUIRE_FALSE(has_warning_been_thrown); 
    REQUIRE(trait_man.GetSize() == 2); 

    // ^^ failing by saying: 
    // Error: Module EvalNK is trying to use trait 'trait_j' of type N3emp6TypeIDE; Previously defined in module(s) EvalNK as type int32_t
    // Even though it's specified as an alternate trait???
    // See Module.hpp for examples of adding altTypes in the AddTrait
  } 
}

TEST_CASE("TraitInfo_Verify", "[core]"){
  { 
    //  [SETUP]
    // Add module(s) to access the trait
    mabe::MABE control(0, NULL);
    control.AddPopulation("test_pop");
    mabe::EvalNK nk_mod(control);
    mabe::EvalNK nk2_mod(control); 

    // Setup a TraitManager
    // Use bools to tell if an error has been thrown 
    bool has_error_been_thrown = false; 
    bool has_warning_been_thrown = false; 

    auto error_func = [&has_error_been_thrown](const std::string & s){
      std::cout << "Error: " << s;
      has_error_been_thrown = true;
    }; 
    auto warning_func = [&has_warning_been_thrown](const std::string & s){
      std::cout << "Warning: " << s;
      has_warning_been_thrown = true; 
    }; 
    mabe::ErrorManager error_man(error_func, warning_func);
    error_man.Activate(); 
    mabe::TraitManager<mabe::ModuleBase> trait_man(error_man);
    trait_man.Unlock(); 

    //  [BEGIN TESTS] 
    // REQUIRED traits must have another trait (SHARED, GENERATED or OWNED) that writes to them

    // Check that if a REQUIRED trait doesn't have one that adds to them, Verify throws error
    trait_man.AddTrait(&nk_mod, mabe::TraitInfo::Access::REQUIRED, "trait_i", "a trait", emp::GetTypeID<int>()); 
    REQUIRE(trait_man.GetSize() == 1);

    trait_man.Verify(true); 
    REQUIRE(has_error_been_thrown); 
    REQUIRE_FALSE(has_warning_been_thrown); 

    // Add a Trait that OWNES the trait, check Verify works
    has_error_been_thrown = false; 
    trait_man.AddTrait(&nk2_mod, mabe::TraitInfo::Access::OWNED, "trait_i", "a trait", emp::GetTypeID<int>()); 
    REQUIRE(trait_man.GetSize() == 1); 

    trait_man.Verify(true); 
    REQUIRE_FALSE(has_error_been_thrown); 
    REQUIRE_FALSE(has_warning_been_thrown); 

    // Do the check again, this time with a trait that GENERATES
    // Check that if a REQUIRED trait doesn't have one that adds to them, Verify throws error
    trait_man.AddTrait(&nk_mod, mabe::TraitInfo::Access::REQUIRED, "trait_j", "a trait", emp::GetTypeID<int>()); 
    REQUIRE(trait_man.GetSize() == 2);

    trait_man.Verify(true); 
    REQUIRE(has_error_been_thrown); 
    REQUIRE_FALSE(has_warning_been_thrown); 

    // Add a Trait that GENERATES the trait, check Verify works
    has_error_been_thrown = false; 
    trait_man.AddTrait(&nk2_mod, mabe::TraitInfo::Access::GENERATED, "trait_j", "a trait", emp::GetTypeID<int>()); 

    trait_man.Verify(true); 
    REQUIRE_FALSE(has_error_been_thrown); 
    REQUIRE_FALSE(has_warning_been_thrown);

    // Do the check again, this time with a trait that SHARES
    // Check that if a REQUIRED trait doesn't have one that adds to them, Verify throws error
    trait_man.AddTrait(&nk_mod, mabe::TraitInfo::Access::REQUIRED, "trait_k", "a trait", emp::GetTypeID<int>()); 
    REQUIRE(trait_man.GetSize() == 3);

    trait_man.Verify(true); 
    REQUIRE(has_error_been_thrown); 
    REQUIRE_FALSE(has_warning_been_thrown); 

    // Add a Trait that GENERATES the trait, check Verify works
    has_error_been_thrown = false; 
    trait_man.AddTrait(&nk2_mod, mabe::TraitInfo::Access::SHARED, "trait_k", "a trait", emp::GetTypeID<int>()); 

    trait_man.Verify(true); 
    REQUIRE_FALSE(has_error_been_thrown); 
    REQUIRE_FALSE(has_warning_been_thrown);
  } 

  {
    //  [SETUP]
    // Add module(s) to access the trait
    mabe::MABE control(0, NULL);
    control.AddPopulation("test_pop");
    mabe::EvalNK nk_mod(control);
    mabe::EvalNK nk2_mod(control); 
    mabe::EvalNK nk3_mod(control);  

    // Setup a TraitManager
    // Use bools to tell if an error has been thrown 
    bool has_error_been_thrown1 = false; 
    bool has_error_been_thrown2 = false; 
    bool has_warning_been_thrown = false; 

    auto error_func1 = [&has_error_been_thrown1](const std::string & s){
      std::cout << "Error: " << s;
      has_error_been_thrown1 = true;
    }; 

    auto error_func2 = [&has_error_been_thrown2](const std::string & s){
      std::cout << "Warning: " << s;
      has_error_been_thrown2 = true; 
    }; 

    auto warning_func = [&has_warning_been_thrown](const std::string & s){
      std::cout << "Warning: " << s;
      has_warning_been_thrown = true; 
    }; 


    mabe::ErrorManager error_man1(error_func1, warning_func);
    mabe::ErrorManager error_man2(error_func2, warning_func); 
    error_man1.Activate(); 
    error_man2.Activate();
    mabe::TraitManager<mabe::ModuleBase> trait_man1(error_man1);
    mabe::TraitManager<mabe::ModuleBase> trait_man2(error_man2);
    trait_man1.Unlock(); 
    trait_man2.Unlock(); 

    //  [BEGIN TESTS] 
    // A trait that is OWNED or GENERATED cannot have other modules writing to it.

    //Create a new OWNED trait
    trait_man1.AddTrait(&nk_mod, mabe::TraitInfo::Access::OWNED, "trait_i", "a trait", emp::GetTypeID<int>()); 
    REQUIRE(trait_man1.GetSize() == 1); 

    // Check Verify throws error if another module tries OWNING it
    trait_man1.AddTrait(&nk2_mod, mabe::TraitInfo::Access::OWNED, "trait_i", "a trait", emp::GetTypeID<int>()); 
    REQUIRE(trait_man1.GetSize() == 1); 

    trait_man1.Verify(true); 
    REQUIRE(has_error_been_thrown1); 
    REQUIRE_FALSE(has_warning_been_thrown); 

    // ------------------------ Must be done with new Manager to avoid earlier error

    //Create a new OWNED trait
    trait_man2.AddTrait(&nk_mod, mabe::TraitInfo::Access::OWNED, "trait_j", "a trait", emp::GetTypeID<int>()); 
    REQUIRE(trait_man2.GetSize() == 1); 

    // Check Verify throws error if another module tries GENERATING it
    trait_man2.AddTrait(&nk2_mod, mabe::TraitInfo::Access::GENERATED, "trait_j", "a trait", emp::GetTypeID<int>()); 
    REQUIRE(trait_man2.GetSize() == 1); 

    trait_man2.Verify(true); 
    REQUIRE(has_error_been_thrown2); 
    REQUIRE_FALSE(has_warning_been_thrown);
 
    // Set up new Managers for GENERATED traits
    // Use new bools to tell if an error has been thrown 
    bool has_error_been_thrown3 = false; 
    bool has_error_been_thrown4 = false; 

    auto error_func3 = [&has_error_been_thrown3](const std::string & s){
      std::cout << "Error: " << s;
      has_error_been_thrown3 = true;
    }; 

    auto error_func4 = [&has_error_been_thrown4](const std::string & s){
      std::cout << "Warning: " << s;
      has_error_been_thrown4 = true; 
    }; 

    mabe::ErrorManager error_man3(error_func3, warning_func);
    mabe::ErrorManager error_man4(error_func4, warning_func); 
    error_man3.Activate(); 
    error_man4.Activate(); 
    mabe::TraitManager<mabe::ModuleBase> trait_man3(error_man3);
    mabe::TraitManager<mabe::ModuleBase> trait_man4(error_man4);
    trait_man3.Unlock(); 
    trait_man4.Unlock(); 

    // ------------------------ Must be done with new Manager to avoid earlier error
   
    //Create a new GENERATED trait
    trait_man3.AddTrait(&nk_mod, mabe::TraitInfo::Access::GENERATED, "trait_k", "a trait", emp::GetTypeID<int>()); 
    REQUIRE(trait_man3.GetSize() == 1); 

    // Add a module that REQUIRES the GENERATED one
    trait_man3.AddTrait(&nk2_mod, mabe::TraitInfo::Access::REQUIRED, "trait_k", "a trait", emp::GetTypeID<int>()); 
    REQUIRE(trait_man3.GetSize() == 1); 

    // Check Verify doesn't throw error because no other module tries to OWN or GENERATE 
    trait_man3.Verify(true); 
    REQUIRE_FALSE(has_error_been_thrown3); 
    REQUIRE_FALSE(has_warning_been_thrown); 

    // Check Verify throws error if another module tries OWNING it
    trait_man3.AddTrait(&nk3_mod, mabe::TraitInfo::Access::OWNED, "trait_k", "a trait", emp::GetTypeID<int>()); 
    REQUIRE(trait_man3.GetSize() == 1); 

    trait_man3.Verify(true); 
    REQUIRE(has_error_been_thrown3); 
    REQUIRE_FALSE(has_warning_been_thrown); 

    // ------------------------ Must be done with new Manager to avoid earlier error

    //Create a new GENERATED trait
    trait_man4.AddTrait(&nk_mod, mabe::TraitInfo::Access::GENERATED, "trait_l", "a trait", emp::GetTypeID<int>()); 
    REQUIRE(trait_man4.GetSize() == 1); 

    // Add a module that REQUIRES the GENERATED one
    trait_man4.AddTrait(&nk2_mod, mabe::TraitInfo::Access::REQUIRED, "trait_l", "a trait", emp::GetTypeID<int>()); 
    REQUIRE(trait_man4.GetSize() == 1); 

    // Check Verify doesn't throw error because no other module tries to OWN or GENERATE 
    trait_man4.Verify(true); 
    REQUIRE_FALSE(has_error_been_thrown4); 
    REQUIRE_FALSE(has_warning_been_thrown); 

    // Check Verify throws error if another module tries GENERATING it
    trait_man4.AddTrait(&nk2_mod, mabe::TraitInfo::Access::GENERATED, "trait_l", "a trait", emp::GetTypeID<int>()); 
    REQUIRE(trait_man4.GetSize() == 1); 

    trait_man4.Verify(true); 
    REQUIRE(has_error_been_thrown4); 
    REQUIRE_FALSE(has_warning_been_thrown);
  }

  {







      
  }
}


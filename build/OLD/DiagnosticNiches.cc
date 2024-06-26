//  This file is part of Empirical, https://github.com/devosoft/Empirical
//  Copyright (C) Michigan State University, 2019.
//  Released under the MIT Software license; see doc/LICENSE
//

#include <iostream>

#include "config/ArgManager.h"
#include "Evolve/World.h"
#include "tools/BitVector.h"
#include "tools/Random.h"

EMP_BUILD_CONFIG( DNConfig,
  GROUP(DEFAULT, "Default settings for Diagnostic Niches model."),
  VALUE(N, uint32_t, 200, "Number of bits in each organisms."), ALIAS(GENOME_SIZE),
  VALUE(SEED, int, 0, "Random number seed (0 for based on time)"),
  VALUE(POP_SIZE, uint32_t, 1000, "Number of organisms in the popoulation."),
  VALUE(MAX_GENS, uint32_t, 2000, "How many generations should we process?"),
  VALUE(MUT_COUNT, uint32_t, 3, "How many bit positions should be randomized?"), ALIAS(NUM_MUTS),
)

using BitOrg = emp::BitVector;

double CalcFitness(const BitOrg & org, size_t pos) {
  emp_assert(pos < org.size());
  if (org[pos] == 0) return 0.0;
  return (double) org.CountZeros();
}

/// Calculate the total fitness of all components summed together.
double CalcTotalFitness(const BitOrg & org) {
  return org.CountOnes() * org.CountZeros();
}

int main(int argc, char* argv[])
{
  DNConfig config;
  config.Read("DiagnosticNiches.cfg");

  auto args = emp::cl::ArgManager(argc, argv);
  if (args.ProcessConfigOptions(config, std::cout, "NK.cfg", "NK-macros.h") == false) exit(0);
  if (args.TestUnknown() == false) exit(0);  // If there are leftover args, throw an error.

  const uint32_t N = config.N();
  const uint32_t POP_SIZE = config.POP_SIZE();
  const uint32_t MAX_GENS = config.MAX_GENS();
  const uint32_t MUT_COUNT = config.MUT_COUNT();

  emp::Random random(config.SEED());

  emp::World<BitOrg> pop(random, "NKWorld");
  pop.SetupFitnessFile().SetTimingRepeat(10);
//  pop.SetupSystematicsFile().SetTimingRepeat(10);
  pop.SetupPopulationFile().SetTimingRepeat(10);
  pop.SetPopStruct_Mixed(true);
  pop.SetCache();

  // Build a random initial population
  for (uint32_t i = 0; i < POP_SIZE; i++) {
    BitOrg next_org(N);
    for (uint32_t j = 0; j < N; j++) next_org[j] = random.P(0.5);
    pop.Inject(next_org);
  }

  // Setup the mutation function.
  std::function<size_t(BitOrg &, emp::Random &)> mut_fun =
    [MUT_COUNT, N](BitOrg & org, emp::Random & random) {
      size_t num_muts = 0;
      for (uint32_t m = 0; m < MUT_COUNT; m++) {
        const uint32_t pos = random.GetUInt(N);
        if (random.P(0.5)) {
          org[pos] ^= 1;
          num_muts++;
        }
      }
      return num_muts;
    };
  pop.SetMutFun( mut_fun );
  pop.SetAutoMutate();

  std::cout << 0 << " : " << pop[0] << " : " << CalcFitness(pop[0], 0) << std::endl;

  std::function<double(BitOrg&)> fit_fun =
    [](BitOrg & org){ return CalcFitness(org,0); };
  pop.SetFitFun( fit_fun );

  // Loop through updates
  for (uint32_t ud = 0; ud < MAX_GENS; ud++) {
    // Print current state.
    // for (uint32_t i = 0; i < pop.GetSize(); i++) std::cout << pop[i] << std::endl;
    // std::cout << std::endl;

    // Keep the best individual.
    emp::EliteSelect(pop, 1, 1);

    // Run a tournament for the rest...
    TournamentSelect(pop, 5, POP_SIZE-1);
    pop.Update();
    std::cout << (ud+1) << " : " << pop[0] << " : " << CalcFitness(pop[0], 0) << std::endl;
  }

}

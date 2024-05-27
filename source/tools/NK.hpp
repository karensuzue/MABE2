/**
 *  @note This file is part of Empirical, https://github.com/devosoft/Empirical
 *  @copyright Copyright (C) Michigan State University, MIT Software license; see doc/LICENSE.md
 *  @date 2019-2024.
 *
 *  @file  NK.hpp
 *  @brief This file provides code to build NK-based algorithms.
 *  @note This file was originally Evolve/NK.h in Epirical.
 *
 *  Two version of landscapes are provided.  NKLandscape pre-calculates the entire landscape, for
 *  easy lookup.  NKLandscapeMemo does lazy evaluation, memorizing values when they're first used.
 *  NKLandscape is faster, but goes up in memory size exponentially with K.  NKLandscapeMemo is
 *  slightly slower, but can handle arbitrarily large landscapes.
 *
 *  @todo Right now we make the library user decide between NKLandscape and NKLandscapeMemo.
 *    Based on K value, we should be able to do this automatically, so we could merge the two.
 */

#ifndef MABE_TOOL_NK_H
#define MABE_TOOL_NK_H

#include "emp/base/vector.hpp"
#include "emp/bits/BitVector.hpp"
#include "emp/functional/memo_function.hpp"
#include "emp/math/math.hpp"
#include "emp/math/Random.hpp"

namespace mabe {

  /// An NK Landscape is a popular tool for studying theoretical questions about evolutionary
  /// dynamics. It is a randomly generated fitness landscape on which bitstrings can evolve.
  /// NK Landscapes have two parameters: N (the length of the bitstrings) and K (epistasis).
  /// Since you have control over the amount of epistasis, NK Landscapes are often called
  /// "tunably rugged" - a useful feature, since the ruggedness of the fitness landscape is thought
  /// to be important to many evolutionary dynamics. For each possible value that a site and its
  /// K neighbors to the right can have, a random fitness contribution is chosen.
  /// These contributions are summed across the bitstring. So when K = 0, each site has a single
  /// optimal value, resulting in a single smooth fitness peak.
  ///
  /// For more information, see Kauffman and Levin,
  /// 1987 (Towards a general theory of adaptive walks on rugged landscapes).
  ///
  /// This object handles generating and maintaining an NK fitness landscape.
  /// Note: Overly large Ns and Ks currently trigger a seg-fault, caused by trying to build a table
  /// that is larger than will fit in memory. If you are using small values for N and K,
  /// you can get better performance by using an NKLandscapeConst instead.

  class NKLandscape {
  private:
    size_t N;             ///< The number of bits in each genome.
    size_t K;             ///< The number of OTHER bits with which each bit is epistatic.
    size_t state_count;   ///< The total number of states associated with each bit table.
    size_t total_count;   ///< The total number of states in the entire landscape space.
    emp::vector< emp::vector<double> > landscape;  ///< The actual values in the landscape.

  public:
    NKLandscape() : N(0), K(0), state_count(0), total_count(0), landscape() { ; }
    NKLandscape(const NKLandscape &) = default;
    NKLandscape(NKLandscape &&) = default;

    /// N is the length of bitstrings in your population, K is the number of neighboring sites
    /// the affect the fitness contribution of each site (i.e. epistasis or ruggedness), random
    /// is the random number generator to use to generate this landscape.
    NKLandscape(size_t _N, size_t _K, emp::Random & random)
     : N(_N), K(_K)
     , state_count(emp::IntPow<size_t>(2,K+1))
     , total_count(N * state_count)
     , landscape(N)
    {
      Reset(random);
    }
    ~NKLandscape() { ; }
    NKLandscape & operator=(const NKLandscape &) = delete;
    NKLandscape & operator=(NKLandscape &&) = default;

    /// Randomize the landscape without changing the landscape size.
    void Reset(emp::Random & random) {
      emp_assert(K < 32, K);
      emp_assert(K < N, K, N);

      // Build new landscape.
      for ( auto & ltable : landscape) {
        ltable.resize(state_count);
        for (double & pos : ltable) {
          pos = random.GetDouble();
        }
      }
    }

    /// Configure for new values of N and K.
    void Config(size_t _N, size_t _K, emp::Random & random) {
      // Save new values.
      N = _N;  K = _K;
      state_count = emp::IntPow<size_t>(2,K+1);
      total_count = N * state_count;
      landscape.resize(N);
      Reset(random);
    }

    /// Returns N
    size_t GetN() const { return N; }
    /// Returns K
    size_t GetK() const { return K; }
    /// Get the number of possible states for a given site
    size_t GetStateCount() const { return state_count; }
    /// Get the total number of states possible in the landscape
    /// (i.e. the number of different fitness contributions in the table)
    size_t GetTotalCount() const { return total_count; }

    /// Get the fitness contribution of position [gene_id] when it (and its K neighbors) have the
    /// value [state]
    double GetFitness(size_t gene_id, size_t state) const {
      emp_assert(state < state_count, state, state_count);
      return landscape[gene_id][state];
    }

    /// Get the fitness of a whole  bitstring
    double GetFitness( std::vector<size_t> states ) const {
      emp_assert(states.size() == N);
      double total = landscape[0][states[0]];
      for (size_t i = 1; i < N; i++) total += GetFitness(i,states[i]);
      return total;
    }

    /// Get the fitness of a whole bitstring (pass by value so can be modified.)
    double GetFitness(emp::BitVector genome) const {
      // Use a double-length genome to easily handle wrap-around.
      genome.Resize(N*2);
      genome |= (genome << N);

      double total = 0.0;
      const size_t mask = emp::MaskLow<size_t>(K+1);
      for (size_t i = 0; i < N; i++) {
        const size_t cur_val = (genome >> i).GetUInt(0) & mask;
	      const double cur_fit = GetFitness(i, cur_val);
        total += cur_fit;
      }
      return total;
    }

    /// Get the fitness of each gene in a bitstring (pass by value so can be modified.)
    emp::vector<double> GetGeneFitnesses(emp::BitVector genome) const {
      emp::vector<double> gene_fitnesses;
      gene_fitnesses.resize(N);

      // Use a double-length genome to easily handle wrap-around.
      genome.Resize(N*2);
      genome |= (genome << N);

      const size_t mask = emp::MaskLow<size_t>(K+1);
      for (size_t i = 0; i < N; i++) {
        const size_t cur_val = (genome >> i).GetUInt(0) & mask;
	      gene_fitnesses[i] = GetFitness(i, cur_val);
      }

      return gene_fitnesses;
    }


    void SetState(size_t n, size_t state, double in_fit) { landscape[n][state] = in_fit; }

    void RandomizeStates(emp::Random & random, size_t num_states=1) {
      for (size_t i = 0; i < num_states; i++) {
        SetState(random.GetUInt(N), random.GetUInt(state_count), random.GetDouble());
      }
    }

  };

  /// The NKLandscapeMemo class is simialar to NKLandscape, but it does not pre-calculate all
  /// of the landscape states.  Instead it determines the value of each gene combination on first
  /// use and memorizes it.

  class NKLandscapeMemo {
  private:
    const size_t N;
    const size_t K;
    mutable emp::vector< emp::memo_function<double(const emp::BitVector &)> > landscape;
    emp::vector<emp::BitVector> masks;

  public:
    NKLandscapeMemo() = delete;
    NKLandscapeMemo(const NKLandscapeMemo &) = delete;
    NKLandscapeMemo(NKLandscapeMemo &&) = default;
    NKLandscapeMemo(size_t _N, size_t _K, emp::Random & random)
      : N(_N), K(_K), landscape(N), masks(N)
    {
      // Each position in the landscape...
      for (size_t n = 0; n < N; n++) {
        // ...should have its own memo_function
        landscape[n] = [&random](const emp::BitVector &){ return random.GetDouble(); };
        // ...and its own mask.
        masks[n].Resize(N);
        for (size_t k = 0; k < K; k++) masks[n][(n+k)%N] = 1;
      }
    }
    ~NKLandscapeMemo() { ; }
    NKLandscapeMemo & operator=(const NKLandscapeMemo &) = delete;
    NKLandscapeMemo & operator=(NKLandscapeMemo &&) = delete;

    size_t GetN() const { return N; }
    size_t GetK() const { return K; }

    double GetFitness(size_t n, const emp::BitVector & state) const {
      emp_assert(state == (state & masks[n]));
      return landscape[n](state);
    }
    double GetFitness(const emp::BitVector & genome) const {
      emp_assert(genome.GetSize() == N);

      // Otherwise calculate it.
      double total = 0.0;
      for (size_t n = 0; n < N; n++) {
        total += landscape[n](genome & masks[n]);
      }
      return total;
    }
  };

}

#endif

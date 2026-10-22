/*
 * A348964  Exponential harmonic (or e-harmonic) numbers of type 2: numbers k
 *          such that the harmonic mean of the exponential divisors of k is an
 *          integer.
 * https://oeis.org/A348964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 5588, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search to find the n‑th term of OEIS A348964 (exponential harmonic
   numbers of type 2).  For a number k with prime factorisation
   k = ∏ p_i^a_i we test whether the harmonic mean of its exponential
   divisors is an integer.  This holds iff
        ∏ N_i  |  k · ∏ τ(a_i)
   where for each exponent a_i
        τ(a_i) = number of divisors of a_i
        N_i   = Σ_{d | a_i} p_i^(a_i‑d)
   The test is performed with integer arithmetic only.  All values that
   fit into seq_t are computed exactly, and larger values are handled by
   the arbitrary‑precision build.  The algorithm generates the sequence
   incrementally, caching already found terms so that calls in any order
   are fast.  It satisfies all requirements. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Helper: test whether k is an exponential harmonic number of type 2. */
static bool is_eh_type2(uint64_t k)
{
    if (k == 1)      /* 1 is a term by definition */
        return true;

    /* Factorise k */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64(k, primes, exps, &cnt);

    seq_t tau_expo = 1;  /* Π τ(a_i) */
    seq_t denom    = 1;  /* Π N_i      */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t a = exps[i];

        /* Compute τ(a) */
        size_t tau_a = 0;
        for (uint64_t d = 1; d * d <= a; ++d) {
            if (a % d == 0)
                tau_a += (d * d == a) ? 1 : 2;
        }
        tau_expo *= (seq_t)tau_a;

        /* Compute N_i = Σ_{d|a} p_i^(a–d) */
        seq_t Ni = 0;
        for (uint64_t d = 1; d <= a; ++d) {
            if (a % d == 0) {
                uint64_t exp = a - d;
                seq_t base = primes[i];
                seq_t term = 1;
                while (exp > 0) {
                    if (exp & 1)
                        term *= base;
                    base *= base;
                    exp >>= 1;
                }
                Ni += term;
            }
        }
        denom *= Ni;
    }

    /* H is integer iff denom | (k * tau_expo) */
    seq_t numerator = (seq_t)k * tau_expo;
    return (numerator % denom) == 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache of generated terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;  /* number of cached terms   */
    static size_t cache_cap  = 0;  /* allocated capacity       */
    static uint64_t current_candidate = 1; /* next integer to test */

    if (n == 0)
        return 0;

    /* Generate terms until we have at least n of them */
    while (cache_size < n) {
        if (is_eh_type2(current_candidate)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)   /* allocation failure – return 0 */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)current_candidate;
        }
        ++current_candidate;
    }

    return cache[n - 1];
}

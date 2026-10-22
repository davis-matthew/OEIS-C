/*
 * A373299  Numbers prime(k) such that prime(k) - prime(k-1) = prime(k+2) -
 *          prime(k+1).
 * https://oeis.org/A373299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 756, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function yields the OEIS sequence A373299.
   It collects prime numbers incrementally and keeps those that satisfy
   the gap equality: p_k - p_{k-1} = p_{k+2} - p_{k+1}.

   The implementation is memoised; results are stored in static
   containers so that calls in any order work.  A small C++ build
   uses std::vector; the pure C build uses malloc / realloc.
   Dependencies are listed in the original description. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term in the list */

#ifdef __cplusplus
    /* ---------- C++ implementation using std::vector ---------- */
    static std::vector<seq_t>       results;
    static std::vector<uint64_t>    primes;
    static uint64_t                 next_candidate = 3;   /* start after 2 */

    if (results.empty()) {
        primes.push_back(2);
    }

    while (results.size() < (size_t)n) {
        uint64_t cand = next_candidate;
        next_candidate += 2;          /* test only odd numbers */

        /* primality test by trial division using known primes */
        bool is_prime = true;
        for (size_t i = 0; i < primes.size(); ++i) {
            uint64_t p = primes[i];
            if (p > cand / p) break;   /* p*p > cand */
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (!is_prime) continue;          /* try next candidate */

        primes.push_back(cand);

        /* check condition for the prime that is now two places before the
           newest prime: index = primes.size() - 3  (k)
           Need primes.size() >= 5 to have k-1 available
        */
        if (primes.size() >= 4) {
            size_t i = primes.size() - 3;           /* index for k */
            uint64_t d1 = primes[i]     - primes[i-1];
            uint64_t d2 = primes[i+2]   - primes[i+1];
            if (d1 == d2) {
                results.push_back( (seq_t)primes[i] );
            }
        }
    }

    return results[(size_t)n - 1];

#else
    /* ---------- C implementation using malloc / realloc ---------- */
    /* Static state shared across calls */
    static bool          initialized      = false;
    static uint64_t      *primes           = NULL;
    static size_t        primes_cap       = 0;
    static size_t        primes_cnt       = 0;
    static seq_t        *results          = NULL;
    static size_t        results_cap      = 0;
    static size_t        results_cnt      = 0;
    static uint64_t      next_candidate   = 3;       /* start from 3 */

    if (!initialized) {
        /* initialise primes list with 2 */
        primes_cap = 16;
        primes     = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0]  = 2;
        primes_cnt = 1;

        /* initialise result cache */
        results_cap = 16;
        results     = (seq_t *)malloc(results_cap * sizeof(seq_t));
        results_cnt = 0;

        initialized = true;
    }

    /* Ensure we have at least n terms in the cache */
    while (results_cnt < n) {
        uint64_t cand = next_candidate;
        next_candidate += 2;          /* skip even numbers */

        /* primality test by trial division using known primes */
        bool isprime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > cand / p) break;      /* p*p > cand */
            if (cand % p == 0) {
                isprime = false;
                break;
            }
        }
        if (!isprime) continue;          /* try next candidate */

        /* store the new prime */
        if (primes_cnt >= primes_cap) {
            size_t newcap = primes_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
            if (!tmp) return (seq_t)0;   /* allocation failure – abort */
            primes     = tmp;
            primes_cap = newcap;
        }
        primes[primes_cnt++] = cand;

        /* Check the gap condition for the prime that is now two places back */
        if (primes_cnt >= 4) {
            size_t i    = primes_cnt - 3;    /* index of candidate prime (k) */
            uint64_t d1 = primes[i]     - primes[i-1];
            uint64_t d2 = primes[i+2]   - primes[i+1];
            if (d1 == d2) {
                /* store the result */
                if (results_cnt >= results_cap) {
                    size_t newcap = results_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(results, newcap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;     /* allocation failure – abort */
                    results     = tmp;
                    results_cap = newcap;
                }
                results[results_cnt++] = (seq_t)primes[i];
            }
        }
    }

    return results[n - 1];
#endif
}

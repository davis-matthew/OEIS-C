/*
 * A257862  a(n) = n if n <= 3, otherwise a(n) is the smallest number not
 *          already used and relatively prime to a(n-1) such that the set of
 *          prime divisors of one of the numbers {a(n-2), a(n)} is a subset of
 *          the set of prime divisors of the other.
 * https://oeis.org/A257862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest unused positive integer, relatively prime to
 *            a(n-1), such that the set of prime factors of a(n-2) is a subset
 *            of the set of prime factors of a(n) or vice versa
 * Range:     u8 n <= 68, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes the smallest prime factor (spf) and the
   rad (product of distinct prime factors) for all numbers up to a fixed
   maximum (5 000 000).  It then generates the sequence on demand, caching
   results.  The algorithm runs in O(n) time for n ≤ 10000. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

#define MAX_VAL 5000000ULL   /* big enough for n ≤ 10000 */

seq_t a(idx_t n)
{
    /* local static data --------------------------------------------------- */
    static bool init = false;
    static seq_t *seq = NULL;          /* cached sequence terms  */
    static size_t seq_cap = 0;         /* number of terms computed */
    static bool *used = NULL;          /* whether a value has been used */
    static uint32_t *spf = NULL;       /* smallest prime factor array   */
    static uint32_t *rad = NULL;       /* rad[n] = product of distinct primes of n */

    if (!init) {
        /* allocate arrays --------------------------------------------------- */
        seq = (seq_t *)malloc((MAX_VAL + 1) * sizeof(seq_t));
        used = (bool *)calloc(MAX_VAL + 1, sizeof(bool));
        spf = (uint32_t *)calloc(MAX_VAL + 1, sizeof(uint32_t));
        rad = (uint32_t *)calloc(MAX_VAL + 1, sizeof(uint32_t));

        /* sieve of Eratosthenes: fill spf --------------------------------- */
        for (idx_t i = 2; i <= MAX_VAL; ++i) {
            if (spf[i] == 0) {          /* i is prime */
                spf[i] = (uint32_t)i;
                if ((uint64_t)i * i <= MAX_VAL) {
                    for (idx_t j = i * i; j <= MAX_VAL; j += i)
                        if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }

        /* compute rad for every number ------------------------------------- */
        rad[1] = 1;
        for (idx_t i = 2; i <= MAX_VAL; ++i) {
            uint32_t p = spf[i];
            uint32_t m = (uint32_t)(i / p);
            if (m % p == 0) rad[i] = rad[m];
            else            rad[i] = rad[m] * p;      /* product of distinct */
        }

        /* initialise first three terms ------------------------------------- */
        seq[1] = 1; seq[2] = 2; seq[3] = 3;
        used[1] = used[2] = used[3] = true;
        seq_cap = 4;   /* next free index */
        init = true;
    }

    /* generate terms up to n ----------------------------------------------- */
    if (n >= seq_cap) {
        for (idx_t i = seq_cap; i <= n; ++i) {
            seq_t prev1 = seq[i - 1];
            seq_t prev2 = seq[i - 2];
            uint32_t radPrev2 = rad[(uint32_t)prev2];   /* prev2 ≤ MAX_VAL for n ≤ 10000 */

            /* search for smallest suitable candidate ----------------------- */
            for (idx_t cand = 1; cand <= MAX_VAL; ++cand) {
                if (used[(size_t)cand]) continue;
                if (gcd(prev1, (seq_t)cand) != 1) continue;

                uint32_t radCand = rad[(uint32_t)cand];
                if ((radCand % radPrev2 == 0) || (radPrev2 % radCand == 0)) {
                    seq[i] = (seq_t)cand;
                    used[(size_t)cand] = true;
                    break;
                }
            }
        }
        seq_cap = n + 1;
    }

    return seq[n];
}

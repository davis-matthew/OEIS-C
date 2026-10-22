/*
 * A350541  Twin primes x, represented by their average, such that x is the
 *          first and x+18 the last of three successive twins.
 * https://oeis.org/A350541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute OEIS A350541: a(n) is the average of the first prime in a triple
 * of successive twin prime pairs where the averages of the first and last
 * twin differ by 18.  This is implemented by an incremental search: each
 * candidate average a is tested for the required primality conditions:
 *
 *   a-1   and  a+1   are primes  (twin pair 1)
 *   a+17  and  a+19  are primes  (twin pair 3)
 *   Either    (a+5  and a+7)   are primes   (twin pair 2 at distance 6)
 *   or        (a+11 and a+13)  are primes   (twin pair 2 at distance 12)
 *
 * For speed we skip odd candidates (all terms are even) and memoise found
 * terms so that calls with arbitrary indices are answered in O(1) after
 * the needed terms have been generated once.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n); */

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache     = NULL;   /* array of a(n) */
    static size_t cache_cap = 0;      /* allocated capacity */
    static size_t cache_len = 0;      /* number of terms stored */

    /* The next candidate average to test (always even).  Initialise after
       the first call. */
    static seq_t next_candidate = 0;   /* 0 means uninitialised */

    if (next_candidate == 0) {
        /* First call: initialise structures. */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_len = 0;
        next_candidate = 12;            /* first known term */
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t a_val = next_candidate;

        /* Quick pre‑check: candidates are even. */
        /* Primality tests */
        if (    isPrimeGeneric(a_val - 1) &&
                isPrimeGeneric(a_val + 1) &&
                isPrimeGeneric(a_val + 17) &&
                isPrimeGeneric(a_val + 19) ) {

            /* Check the middle twin pair in one of the two possible ways */
            bool middle_ok =
                (isPrimeGeneric(a_val + 5)   && isPrimeGeneric(a_val + 7)) ||
                (isPrimeGeneric(a_val + 11)  && isPrimeGeneric(a_val + 13));

            if (middle_ok) {
                /* Store the found term */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = a_val;
            }
        }

        /* Advance to next candidate (skip odds). */
        next_candidate += 2;
    }

    return cache[n - 1];
}

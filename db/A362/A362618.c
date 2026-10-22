/*
 * A362618  Numbers whose prime factorization has either (1) odd length, or
 *          (2) equal middle parts.
 * https://oeis.org/A362618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   factor_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose prime factorization has either (1) an odd
 * length, or (2) equal middle prime factors (OEIS A362618).  The
 * function searches successive integers from 2 upwards, checks the
 * property using trial division, stores the qualifying numbers in a
 * static cache (so calls can arrive in any order), and returns the
 * n‑th term. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_trial.h"

/* --- main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t *cache = NULL;          /* array of qualifying numbers */
    static size_t  cache_size = 0;       /* number of elements stored */
    static size_t  cache_cap   = 0;       /* allocated capacity      */
    static idx_t   next_candidate = 2;   /* next integer to test    */

    /* expand cache until we have at least n elements */
    while (cache_size < n)
    {
        uint64_t val = (uint64_t)next_candidate;

        /* factor val with trial division into arrays of primes & exponents */
        uint64_t primes[64];
        uint64_t exps  [64];
        size_t   num_primes = 0;
        factor_trial(val, primes, exps, &num_primes);

        /* compute total number of prime factors (multiplicity) */
        size_t len = 0;
        for (size_t i = 0; i < num_primes; ++i)
            len += (size_t)exps[i];

        bool ok = false;
        if (len & 1U)               /* odd length -> qualifies */
        {
            ok = true;
        }
        else                         /* even length: check middle parts */
        {
            size_t target1 = len / 2U - 1U;
            size_t target2 = len / 2U;
            size_t cum = 0U;

            for (size_t i = 0; i < num_primes; ++i)
            {
                size_t block_end = cum + (size_t)exps[i];
                if (target1 < block_end && target2 < block_end) {
                    /* both middle positions lie in the same prime block */
                    ok = true;
                    break;
                }
                if (target1 < block_end && target2 >= block_end) {
                    /* middle positions lie in different blocks */
                    ok = false;
                    break;
                }
                cum = block_end;
            }
        }

        if (ok)
        {
            /* ensure capacity */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2U : 1U;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)val;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}

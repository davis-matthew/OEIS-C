/*
 * A384073  Numbers k such that d(k)^d(k) == d(k) (mod k), where d = A000005.
 * https://oeis.org/A384073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k such that d(k)^d(k) ≡ d(k) (mod k)
 * Range:     u8 n <= 14, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   divisor_count_u64, ensure_capacity, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  d = divisor count (OEIS A000005) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/mod_pow.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* cached values of the sequence */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of terms stored    */
    static size_t cache_cap  = 0;   /* allocated capacity        */

    /* next integer to test (starts at 2) */
    static uint64_t cand = 2;

    /* grow cache until we have at least n terms */
    while (cache_len < (size_t)n)
    {
        /* search for the next qualifying integer */
        while (1)
        {
            /* divisor count of candidate */
            uint64_t d = divisor_count_u64(cand);

            /* d^d mod cand, all in seq_t to keep values in range */
            seq_t pow = mod_pow((seq_t)d, (uint64_t)d, (seq_t)cand);

            if ((seq_t)d == pow)      /* condition satisfied */
            {
                /* enlarge cache if necessary */
                if (cache_len + 1 > cache_cap)
                    ensure_capacity(cache_len + 1, &cache_cap, (void**)&cache, sizeof(seq_t));

                cache[cache_len++] = (seq_t)cand;   /* store the term */
                ++cand;                            /* next candidate */
                break;                             /* ready for next term */
            }
            ++cand;          /* try next candidate */
        }
    }

    return cache[n - 1];
}

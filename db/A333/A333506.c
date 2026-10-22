/*
 * A333506  Numbers k that divide 19^k-1.
 * https://oeis.org/A333506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 1290, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   modpow_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A333506
   a(n) = the n‑th positive integer k for which k | 19^k − 1.
   Computation: scan integers k = 1,2,… and test 19^k ≡ 1 (mod k). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/modpow_ull.h"

/* Cache the sequence values so that calls may be in arbitrary order. */
static seq_t  *cache   = NULL;   /* 1‑based index array */
static size_t  cache_len = 0;    /* number of stored elements   */
static size_t  cache_cap = 0;    /* allocated capacity         */
static idx_t   tested    = 0;    /* largest integer already tested */

/* Return the nth element of the sequence. */
seq_t a(idx_t n)
{
    /* Extend cache until it contains at least n elements. */
    while (cache_len < n)
    {
        tested++;                       /* next integer to examine       */
        idx_t k = tested;               /* candidate integer */
        bool ok;

        if (k == 1)                     /* 1 divides 19^1−1 by definition */
            ok = true;
        else
        {
            /* 19^k mod k == 1 ?  Use fast 64‑bit modular exponentiation. */
            uint64_t mod_res = modpow_ull(19, (uint64_t)k, (uint64_t)k);
            ok = (mod_res == 1ULL);
        }

        if (ok)
        {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) /* memory failure – return 0 (unlikely in tests) */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;   /* store value (cast is safe). */
        }
    }

    return cache[n-1]; /* cache is 1‑based, array is 0‑based. */
}

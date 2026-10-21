/*
 * A258844  Numbers k with the property that it is possible to write the base
 *          2 expansion of k as concat(a_2,b_2), with a_2>0 and b_2>0 such
 *          that, converting a_2 and b_2 to base 10 as a and b, we have
 *          (a+b)^2 = k.
 * https://oeis.org/A258844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A258844: squares m² such that the binary representation of m² can be split
 *  into two non‑empty parts a₂ and b₂; interpreting a₂ and b₂ as binary
 *  numbers gives integers a and b with a + b = m.  The function generates
 *  the terms on demand and caches them.
 *  Formula (definition): a(n) = m² where m is the n‑th integer with the
 *  property described above.  Unneeded terms are ignored. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* static cache of generated terms */
    static seq_t *cache   = NULL;
    static size_t  cache_cap = 0;
    static size_t  cache_len = 0;
    static idx_t   next_m   = 2;        /* next m to test (start from 2) */

    /* initialise cache */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t k = ((seq_t)next_m) * ((seq_t)next_m);      /* m² */
        size_t L = bitlen_seq_t(k);                      /* binary length */

        bool good = false;
        for (size_t lb = 1; lb < L && !good; ++lb) {
            seq_t mask = (((seq_t)1) << lb) - 1;          /* lower lb bits */
            seq_t b = k & mask;
            seq_t a = k >> lb;                          /* upper bits */

            /* test the condition a > 0, b > 0, a + b == m */
            if (a > 0 && b > 0 && (a + b) == (seq_t)next_m) {
                good = true;
            }
        }

        if (good) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = k;
        }

        ++next_m;
    }

    return cache[n - 1];
}

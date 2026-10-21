/*
 * A256114  Numbers n such that digit_product(n^2) = (digit_product(n))^2 and
 *          n mod 10 > 0.
 * https://oeis.org/A256114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 9662, u64 n <= 9662,
 *            u128 n <= 9662, big any n
 * Helpers:   digit_product, seq_alloc, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by brute‑force search over positive integers, skipping multiples of 10. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_product.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *terms = NULL;
    static size_t sz = 0;      /* number of terms in cache      */
    static size_t cap = 0;     /* allocated capacity            */
    static idx_t last_tested = 0;  /* last integer tested           */

    /* grow the cache until we have at least n terms */
    while (sz < n) {
        /* next candidate integer */
        ++last_tested;
        /* skip numbers ending in 0 */
        if (last_tested % 10 == 0) {
            continue;
        }

        /* convert to seq_t */
        seq_t cand = (seq_t)last_tested;

        /* compute digit products */
        seq_t dp   = digit_product(cand);               /* digit_product(n)   */
        seq_t sq   = seq_mul(cand, cand);               /* n^2                */
        seq_t dp_sq = seq_mul(dp, dp);                  /* (dp)^2             */
        seq_t dp_n2 = digit_product(sq);                /* dp(n^2)            */

        /* test the property */
        if (dp_n2 == dp_sq) {
            /* ensure capacity */
            if (sz == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                if (terms == NULL) {
                    terms = seq_alloc(new_cap);
                } else {
                    terms = seq_realloc(terms, cap, new_cap);
                }
                cap = new_cap;
            }
            terms[sz++] = cand;
        }
    }
    return terms[n - 1];
}

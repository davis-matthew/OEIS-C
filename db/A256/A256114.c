/*
 * A256114  Numbers n such that digit_product(n^2) = (digit_product(n))^2 and
 *          n mod 10 > 0.
 * https://oeis.org/A256114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 15212, u64 n <= 15213,
 *            u128 n <= 15213, big any n
 * Validated: standard, pass, 67 terms through n = 9662 (2026-11-01)
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by brute‑force search over positive integers, skipping multiples of 10. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/digit_product.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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

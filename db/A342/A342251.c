/*
 * A342251  Number of ways to write n as an ordered sum of eight powers of 2.
 * https://oeis.org/A342251
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 213, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS:A342251
   Formula: a(n) = number of ways to write n as an ordered sum of
   eight powers of 2 (coefficient of x^n in (∑_{k≥0}x^{2^k})^8). */

#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

/* linear‑time dynamic programming; works for any n ≥ 8.
   For n < 8 the definition is not in the OEIS range, so return 0. */
seq_t a(idx_t n)
{
    if (n < 8) return 0;

    /* collect powers of two ≤ n */
    idx_t powers[64];
    int pc = 0;
    for (idx_t pw = 1; pw <= n; pw <<= 1) {
        powers[pc++] = pw;
    }

    /* the number of positions in the sum                     */
    const int positions = 8;

    /* static buffer for small n (≤ 10000) – fast and safe for
       trivial seq_t types. */
    enum { MAX_STATIC = 10000 };
    static seq_t static_cur[MAX_STATIC + 1];
    static seq_t static_next[MAX_STATIC + 1];

    /* pointers to working buffers */
    seq_t *cur, *next;
    int use_static = (n <= MAX_STATIC);

    if (use_static) {
        cur  = static_cur;
        next = static_next;
    } else {
#ifdef __cplusplus
        static std::vector<seq_t> vec_cur, vec_next;
        vec_cur.assign((size_t)(n + 1), 0);
        vec_next.assign((size_t)(n + 1), 0);
        cur  = vec_cur.data();
        next = vec_next.data();
#else
        cur  = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        next = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!cur || !next) return 0;   /* allocation failure unlikely */
        for (idx_t i = 0; i <= n; ++i) {
            cur[i]  = 0;
            next[i] = 0;
        }
#endif
    }

    /* initialize 0‑sum with one empty tuple */
    for (idx_t i = 0; i <= n; ++i) {
        cur[i]  = 0;
        next[i] = 0;
    }
    cur[0] = 1;

    /* iterate over the eight positions */
    for (int pos = 0; pos < positions; ++pos) {
        /* accumulate contributions from each power of two */
        for (int p = 0; p < pc; ++p) {
            idx_t power = powers[p];
            for (idx_t s = power; s <= n; ++s) {
                next[s] += cur[s - power];
            }
        }
        /* copy next → cur and reset next for the next iteration */
        for (idx_t i = 0; i <= n; ++i) {
            cur[i]  = next[i];
            next[i] = 0;
        }
    }

    seq_t result = cur[n];

    if (!use_static) {
#ifndef __cplusplus
        free(cur);
        free(next);
#endif
    }

    return result;
}

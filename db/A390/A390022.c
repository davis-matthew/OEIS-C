/*
 * A390022  Values of v in the quartets (3, u, v, w) of type 3; i.e., values
 *          of v for solutions to 3*(3 - u) = v*(v - w), in distinct positive
 *          integers, with v > 1, sorted by nondecreasing values of u; see
 *          A390021.
 * https://oeis.org/A390022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   sort_seqt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A390022: values of v in the quartets (3,u,v,w) of type 3;
   solutions satisfy 3*(3-u) = v*(v-w) with distinct positive integers
   3, u, v, w, v>1.  The sequence lists the v’s sorted by nondecreasing u. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sort_seqt.h"

/* cache for the sequence */
static seq_t *seq_buf = NULL;
static size_t seq_len = 0;   /* number of stored terms */
static size_t seq_cap = 0;   /* capacity of seq_buf */
static uint64_t last_u = 0;  /* last processed value of u (skipping u==3) */

/* ensure capacity for seq_buf */
static void seq_grow(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* produce the n-th term (1‑based) of the sequence */
seq_t a(idx_t n)
{
    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        ++last_u;                      /* next u */
        if (last_u == 3) continue;    /* u must be distinct from 3 */

        /* compute R = 3*(3-u) */
        int64_t R = (int64_t)3 * ((int64_t)3 - (int64_t)last_u);
        if (R == 0) continue;          /* never happens since u != 3 */

        int64_t absR = R > 0 ? R : -R;
        if (absR == 0) continue;

        /* buffer to hold v’s for this u */
        seq_t temp[256];
        size_t temp_len = 0;

        /* iterate over divisors of absR */
        for (uint64_t d = 1; d * d <= (uint64_t)absR; ++d) {
            if (absR % d != 0) continue;

            /* first divisor d */
            int64_t v = (int64_t)d;
            if (v > 3 && v != (int64_t)last_u) {            /* v>3, distinct from u */
                int64_t w = v - R / v;                     /* w = v - R/v */
                if (w > 0 && w != 3 &&             /* w positive, not 3 (distinct from 3) */
                    w != (int64_t)last_u &&       /* w distinct from u */
                    w != v) {                     /* w distinct from v */
                    if (temp_len < 256)
                        temp[temp_len++] = (seq_t)v;
                }
            }

            /* complementary divisor if different */
            uint64_t d2 = absR / d;
            if (d2 != d) {
                v = (int64_t)d2;
                if (v > 3 && v != (int64_t)last_u) {
                    int64_t w = v - R / v;
                    if (w > 0 && w != 3 &&
                        w != (int64_t)last_u &&
                        w != v) {
                        if (temp_len < 256)
                            temp[temp_len++] = (seq_t)v;
                    }
                }
            }
        }

        if (temp_len == 0) continue;    /* no valid v for this u */

        /* sort the collected v’s for this u */
        sort_seqt(temp, temp_len);

        /* append them to the global sequence */
        size_t need = seq_len + temp_len;
        seq_grow(need);
        for (size_t i = 0; i < temp_len; ++i)
            seq_buf[seq_len + i] = temp[i];
        seq_len += temp_len;
    }

    return seq_buf[n - 1];          /* offset 1 */
}

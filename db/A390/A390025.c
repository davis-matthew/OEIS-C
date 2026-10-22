/*
 * A390025  Values of v in the quartets (4, u, v, w) of type 3; i.e., values
 *          of v for solutions to 4*(4 - u) = v*(v - w), in distinct positive
 *          integers, with v > 1, sorted by nondecreasing values of u; see
 *          A390024.
 * https://oeis.org/A390025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390025: values of v in the quartets (4,u,v,w) of type 3.
   For each integer u ≠ 4 (ascending), solve
          4*(4 - u) = v*(v - w)
   in distinct positive integers 4,u,v,w with v > 1.
   All admissible v’s for a given u are output in ascending order.
   The sequence a(n) is the concatenation of these lists for u = 1,2,3,…,
   with u = 4 contributing no terms. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of values produced so far */

    for (idx_t u = 1; ; ++u) {
        if (u == 4) continue;   /* u = 4 gives no admissible solutions */

        /* L = 4*(4 - u) ;  may be negative */
        int64_t L = (int64_t)16 - (int64_t)4 * (int64_t)u;
        if (L == 0) continue;   /* no solutions when L = 0 */

        int64_t m = L > 0 ? L : -L;   /* |L| */

        /* collect admissible v's for this u */
        seq_t vals[256];
        size_t vcnt = 0;

        for (int64_t d = 1; d * d <= m; ++d) {
            if (m % d != 0) continue;

            /* v candidates: d and m/d */
            for (int k = 0; k < 2; ++k) {
                int64_t v = (k == 0) ? d : m / d;
                if (v <= 4 || v == (int64_t)u) continue;  /* v must be >4 and distinct */

                int64_t s = L / v;                /* s = v - w  */
                if (s == 0) continue;             /* would give w = v, violating distinctness */

                int64_t w = v - s;
                if (w <= 4 || w == v || w == (int64_t)u) continue;  /* w >4, distinct */

                vals[vcnt++] = (seq_t)v;
            }
        }

        /* sort the collected v's in ascending order */
        for (size_t i = 1; i < vcnt; ++i) {
            seq_t key = vals[i];
            size_t j = i;
            while (j > 0 && vals[j - 1] > key) {
                vals[j] = vals[j - 1];
                --j;
            }
            vals[j] = key;
        }

        /* output v's, skipping duplicates for this u */
        for (size_t i = 0; i < vcnt; ++i) {
            if (i > 0 && vals[i] == vals[i - 1]) continue;  /* deduplicate */
            ++count;
            if (count == n) return vals[i];
        }
    }
}

/*
 * A348382  Number of compositions of n that are not a twin (x,x) but have
 *          adjacent equal parts.
 * https://oeis.org/A348382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{n-1} - B(n) - (n % 2 == 0 ? 1 : 0) * where B(0)=1 and
 *            for n>0 B(n) = Σ_{k=1..n} T(k) * B(n-k) * with T(k) = Σ_{d|k}
 *            (-1)^{d-1}. This is derived from * the generating functions for
 *            compositions with adjacent equal parts
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* static workspace */
    static idx_t limit = 0;
    static seq_t *B = NULL;   /* B[0..limit] */
    static int   *T = NULL;   /* T[0..limit]  (T[0] unused) */

    /* enlarge workspace if necessary */
    if (n > limit) {
        idx_t new_lim = n;

        /* resize B array */
        B = (seq_t *)realloc(B, (new_lim + 1) * sizeof(seq_t));
        /* resize T array */
        T = (int *)realloc(T, (new_lim + 1) * sizeof(int));

        /* recompute T[1..new_lim] */
        for (idx_t i = 0; i <= new_lim; ++i)
            T[i] = 0;
        for (idx_t d = 1; d <= new_lim; ++d) {
            int val = (d & 1) ? 1 : -1;
            for (idx_t m = d; m <= new_lim; m += d)
                T[m] += val;
        }

        /* initialise B[0] */
        B[0] = 1;

        /* compute B[limit+1 .. new_lim] */
        for (idx_t k = limit + 1; k <= new_lim; ++k) {
            seq_t pos = 0;
            seq_t neg = 0;
            for (idx_t i = 1; i <= k; ++i) {
                int ti = T[i];
                if (ti > 0) {
                    pos += B[k - i] * (seq_t)ti;
                } else if (ti < 0) {
                    neg += B[k - i] * (seq_t)(-ti);
                }
            }
            B[k] = pos - neg;
        }

        limit = new_lim;
    }

    /* total number of compositions of n is 2^{n-1} */
    seq_t total = pow2(n - 1);

    /* compositions with at least one adjacent equal part */
    seq_t with_adj = total - B[n];

    /* subtract the twin (x,x) which exists only for even n */
    seq_t twin = (n & 1u) ? 0 : 1;

    return with_adj - twin;
}

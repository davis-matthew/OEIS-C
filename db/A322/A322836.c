/*
 * A322836  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          A(n,k) is Chebyshev polynomial of the first kind T_{n}(x),
 *          evaluated at x=k.
 * https://oeis.org/A322836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T_i(j) where (i,j) are antidiagonal coordinates of n, *
 *            with A(i,j) = ChebyshevT(i)(j). The recurrence used is *
 *            T_0(j)=1, T_1(j)=j, T_m(j)=2*j*T_{m-1}(j)-T_{m-2}(j)
 * Range:     i8 n <= 30, i16 n <= 59, i32 n <= 128, i64 n <= 316,
 *            i128 n <= 847, big any n
 * Helpers:   _find_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_find_t.h"

seq_t a(idx_t n)
{
    /* Antidiagonal coordinates */
    idx_t t   = _find_t(n);
    idx_t pos = n - (t * (t + 1)) / 2;
    idx_t i   = pos;           /* Chebyshev index (row) */
    idx_t j   = t - pos;       /* Argument to Chebyshev polynomial */

    seq_t sj = (seq_t)j;

    if (i == 0) return 1;
    if (i == 1) return sj;

    /* Recurrence for Chebyshev polynomials */
    seq_t prev2 = 1;   /* T_0(j) */
    seq_t prev1 = sj;  /* T_1(j) */
    for (idx_t m = 2; m <= i; ++m)
    {
        /* Use 128–bit arithmetic to avoid overflow of the intermediate
           product 2*j*prev1 when seq_t is 64‑bit. */
        seq_t cur = (seq_t)((( (__int128)2 * sj) * prev1) - prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

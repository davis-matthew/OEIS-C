/*
 * A368759  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = n! * (1 + Sum_{j=0..n} j^k/j!).
 * https://oeis.org/A368759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(n,k) = n^k + n*T(n-1,k) with T(0,k)=1+δ(k,0).
   a(n) is obtained by reading the square array T(n,k) along
   antidiagonals (downwards).  The function below directly
   computes a(n) for any unsigned 64‑bit index n. */
#include <stdint.h>
#include <math.h>

/* Compute a(n) for the OEIS sequence A368759. */
seq_t a(idx_t n)
{
    /* --- Find diagonal index d such that preceding terms are <= n --- */
    idx_t d = 0;
    /* Find smallest d with (d+1)*(d+2)/2 > n.  d is O(sqrt(n)).   */
    while (((d + 1) * (d + 2)) / 2 <= n) d++;

    /* --- Locate position inside this diagonal --- */
    idx_t offset = n - d * (d + 1) / 2;   /* 0 <= offset <= d          */
    idx_t i = offset;                     /* row index                  */
    idx_t j = d - offset;                 /* column index               */

    /* --- Evaluate T(i,j) --- */
    seq_t t_prev = (j == 0) ? (seq_t)2 : (seq_t)1;   /* T(0,j)               */

    for (idx_t nn = 1; nn <= i; ++nn)
    {
        seq_t nn_val = (seq_t)nn;
        seq_t pow_val = (seq_t)1;
        /* compute n^j  (j <= d is small, at most ~140 for n ≤ 10000) */
        for (idx_t kk = 0; kk < j; ++kk)
            pow_val *= nn_val;

        t_prev = pow_val + nn_val * t_prev; /* recurrence */
    }

    return t_prev;   /* T(i,j) is the desired a(n) */
}

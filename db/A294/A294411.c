/*
 * A294411  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. -exp(k*x)*LambertW(-x).
 * https://oeis.org/A294411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 34, u32 n <= 76, u64 n <= 169,
 *            u128 n <= 433, big any n
 * Helpers:   binom, diag_from_n, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A294411
 *
 * Square array A(n,k) read by antidiagonals.  The column k (k ≥ 0) has the
 * exponential generating function
 *
 *     A_k(x) = -exp(k*x) * LambertW(-x)
 *
 *  Expanding gives
 *
 *     A(n,k) = ∑_{m=1}^{n} binom(n,m) * m^{m-1} * k^{n-m}.
 *
 *  The sequence a(n) is the reading of this array by antidiagonals:
 *  a(0)=A(0,0), a(1)=A(0,1), a(2)=A(1,0), a(3)=A(0,2), ...
 *
 *  This function computes a(n) for any n ≥ 0.  It uses the helper functions
 *  `binom(idx_t a, idx_t b)` and `seq_pow(seq_t base, idx_t exp)`.
 */

#include "../utils/binom.h"
#include "../utils/seq_pow.h"
#include "../utils/diag_from_n.h"

seq_t
a(idx_t n)
{
    /* Step 1: determine the (row,col) coordinates in the square array */
    idx_t s = diag_from_n(n);                 /* diagonal number */
    idx_t tri_prev = s * (s + 1) / 2;         /* elements before this diagonal */
    idx_t pos = n - tri_prev;                 /* position inside the diagonal 0 … s */
    idx_t row = pos;                          /* n‑index (row) */
    idx_t k   = s - pos;                      /* k‑index (column) */

    /* Element on a triangular number diagonal is always 0 (row==0) */
    if (row == 0)
        return (seq_t)0;

    /* Step 2: evaluate A(row,k) = Σ_{m=1}^{row} binom(row,m)·m^{m−1}·k^{row−m} */
    seq_t sum = 0;
    for (idx_t m = 1; m <= row; ++m) {
        seq_t term = binom(row, m);
        term = term * seq_pow((seq_t)m, m - 1);            /* m^{m-1}          */
        term = term * seq_pow((seq_t)k, row - m);          /* k^{row-m}        */
        sum += term;
    }
    return sum;
}

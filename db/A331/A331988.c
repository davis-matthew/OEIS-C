/*
 * A331988  Table T(n,k) read by antidiagonals. T(n,k) is the maximum value of
 *          Product_{i=1..n} Sum_{j=1..k} r_j[i] where each r_j is a
 *          permutation of {1..n}.
 * https://oeis.org/A331988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the antidiagonal coordinates of n:
 *            let d be minimal with d*(d+1)/2 >= n, offset = n - (d-1)*d/2 -
 *            1, i = d - offset, j = offset + 1. T(i,j) = i! if j == 1
 *            ((j*(i+1))/2)^i if j*(i+1) is even f^(i-α) * (f+1)^α, otherwise,
 *            f = floor(j*(i+1)/2), α = j*i*(i+1)/2 - i*f
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 56, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined for offset 0, return 0 */

    /* Find minimal d with d*(d+1)/2 >= n */
    idx_t d = 1;
    idx_t tri = 1;                      /* triangular number d*(d+1)/2 */
    while (tri < n) {
        d++;
        tri += d;                       /* update to next triangle */
    }
    idx_t tri_prev = tri - d;           /* (d-1)*d/2 */
    idx_t offset   = n - tri_prev - 1;  /* 0‑based offset in diagonal d */
    idx_t i = d - offset;               /* row index (1‑based) */
    idx_t j = offset + 1;               /* column index (1‑based) */

    /* Handle the special case j==1 (single permutation) */
    if (j == 1) {
        return factorial(i);
    }

    /* General case */
    seq_t n_val = (seq_t)i;
    seq_t k_val = (seq_t)j;
    seq_t n_plus1 = n_val + 1;
    seq_t numer = k_val * n_plus1;        /* j * (i+1) */

    if ((numer & 1) == 0) {              /* even case */
        seq_t mid = numer / 2;
        return big_pow(mid, n_val);
    }

    /* odd case */
    seq_t f = (numer - 1) / 2;           /* floor(j*(i+1)/2)   */
    seq_t f_plus = f + 1;                /* ceil(j*(i+1)/2)    */
    seq_t total = k_val * n_val * n_plus1 / 2; /* j*i*(i+1)/2 */
    seq_t alpha = total - n_val * f;      /* number of rows with ceil sum */
    seq_t pow1 = big_pow(f, n_val - alpha);
    seq_t pow2 = big_pow(f_plus, alpha);
    return pow1 * pow2;
}

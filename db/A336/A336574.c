/*
 * A336574  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          T(n,k) = Sum_{j=0..n} 2^j * binomial(n,j) *
 *          binomial(k*n+j+1,n)/(k*n+j+1).
 * https://oeis.org/A336574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{j≥0..n} 2^j * binomial(n,j) *
 *            binomial(k*n+j+1,n)/(k*n+j+1), where (n,k) are the indices read
 *            by antidiagonals of the square array
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 85, u64 n <= 202,
 *            u128 n <= 518, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Compute T(n,k) from the definition above */
static seq_t T_val(idx_t n, idx_t k)
{
    if (n == 0) return (seq_t)1;

    seq_t sum  = 0;
    seq_t pow2 = 1;          /* 2^j */
    for (idx_t j = 0; j <= n; ++j) {
        seq_t term = binom(n, j);
        idx_t denom_idx = k * n + j + 1;
        term = term * binom(denom_idx, n);
        term = term / (seq_t)denom_idx;
        term = term * pow2;
        sum += term;
        pow2 <<= 1;          /* multiply by 2 for next j */
    }
    return sum;
}

/* Convert the linear index into (n,k) coming from the antidiagonal
   traversal of the square array and return T(n,k). */
seq_t a(idx_t idx)
{
    idx_t s = 0;                 /* current diagonal sum n+k = s */
    idx_t count_before = 0;      /* number of elements before diagonal s */

    /* Find the diagonal that contains idx */
    while (idx >= count_before + (s + 1)) {
        count_before += s + 1;   /* each diagonal has s+1 elements */
        ++s;
    }

    idx_t offset = idx - count_before;   /* position within diagonal */
    idx_t n_val = offset;                /* row index */
    idx_t k_val = s - offset;            /* column index */

    return T_val(n_val, k_val);
}

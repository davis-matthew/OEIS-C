/*
 * A369018  Triangle read by rows: T(n, k) = binomial(n, k - 1)*(k - 1)^(k -
 *          1)*n*(n - k + 1)^(n - k).
 * https://oeis.org/A369018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle read by rows of * T(n,k) = binomial(n, k-1)
 *            * (k-1)^(k-1) * n * (n-k+1)^(n-k) * (with 0^0 treated as 0). The
 *            triangle is ordered as: * 0, n^n, and then T(n,2), T(n,3), …,
 *            T(n,n) for each row n ≥ 1
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   _floor_isqrt_u64, big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

/* Compute the nth term of OEIS A369018 */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = (_floor_isqrt_u64(8*n + 1) - 1) / 2;
    idx_t row_start = r * (r + 1) / 2;
    idx_t pos_in_row = n - row_start;  /* 0‑based position */

    /* Row 0 contains only 0 */
    if (r == 0) {
        return 0;
    }

    /* First element of the row */
    if (pos_in_row == 0) {
        return 0;
    }

    /* Second element: n^n */
    if (pos_in_row == 1) {
        seq_t nn = (seq_t)r;
        return big_pow(nn, nn);
    }

    /* Remaining elements: compute T(r, k) with k = pos_in_row */
    idx_t k          = pos_in_row;        /* k ranges from 2 to r */
    seq_t n_seq      = (seq_t)r;
    seq_t k_minus    = (seq_t)(k - 1);
    seq_t binom_val  = binomial(r, k - 1);

    seq_t pow1 = big_pow(k_minus, k_minus);                     /* (k-1)^(k-1) */
    seq_t base2 = (seq_t)(r - k + 1);                           /* (n-k+1) */
    seq_t exp2  = (seq_t)(r - k);                                /* (n-k)   */
    seq_t pow2 = big_pow(base2, exp2);                           /* (n-k+1)^(n-k) */

    seq_t res = binom_val;
    res = seq_mul(res, pow1);
    res = seq_mul(res, n_seq);
    res = seq_mul(res, pow2);

    return res;
}

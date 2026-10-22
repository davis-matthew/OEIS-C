/*
 * A363399  Triangle read by rows. T(n, k) = [x^k] P(n, x), where P(n, x) =
 *          Sum_{k=0..n} 2^(n - k) * Sum_{j=0..k} (x^j * binomial(k, j) * (j +
 *          1)^n), (tangent case).
 * https://oeis.org/A363399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = the row index satisfying n <
 *            (r+1)(r+2)/2, k = n - r(r+1)/2 (the column within that row), and
 *            T(r,k) = (k+1)^r * ( 2^(r+1) - Σ_{j=0}^k C(r+1, j) ). This is
 *            the triangle T(n,k) read row‑by‑row
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* determine which row n lies in */
    idx_t r = 0, cnt = 0;
    for (;; ++r) {
        if (n < cnt + (r + 1))
            break;
        cnt += r + 1;
    }
    idx_t k = n - cnt;

    /* (k+1)^r */
    seq_t power = pow_seq((seq_t)(k + 1), (int)r);

    /* 2^(r+1) */
    seq_t two_pow = pow_seq((seq_t)2, (int)(r + 1));

    /* sum of binomial coefficients up to k */
    seq_t sum_bin = (seq_t)0;
    for (idx_t j = 0; j <= k; ++j)
        sum_bin += binomial(r + 1, j);

    seq_t part = two_pow - sum_bin;
    return power * part;
}

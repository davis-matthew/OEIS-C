/*
 * A330843  Square array T(n,k) = [x^n] ((1+x)^(k+1) / (1-x)^(k-1))^n, n>=0,
 *          k>=0, read by descending antidiagonals.
 * https://oeis.org/A330843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is obtained by reading the square array T(n,k) (descending
 *            antidiagonals) where T(n,k) = Σ_{j=0..n} binomial((k+1)*n, j) *
 *            binomial(k*n - j - 1, n - j) The mapping from index n to the
 *            pair (row, col) is: let s = ⌊(√(8n+1) - 1)/2⌋ let offset = n -
 *            s*(s+1)/2 then a(n) = T(offset, s - offset)
 * Range:     i8 n <= 17, i16 n <= 39, i32 n <= 85, i64 n <= 203,
 *            i128 n <= 552, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer square root for 64‑bit unsigned */
static inline idx_t int_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x / 2 + 1;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) lo = mid + 1;
        else        hi = mid - 1;
    }
    return hi;
}

/* binomial with possible negative upper argument */
static inline seq_t binom_general(seq_t top, idx_t k)
{
    if (k == 0)
        return 1;
    if (top >= 0) {
        return binomial((idx_t)top, k);
    } else {
        seq_t m = -top;
        seq_t res = binomial((idx_t)(m + k - 1), k);
        if (k & 1)
            res = -res;
        return res;
    }
}

seq_t a(idx_t n)
{
    /* find s and the offsets */
    idx_t tmp = 8 * n + 1;
    idx_t sqrt_tmp = int_sqrt_u64(tmp);
    idx_t s = (sqrt_tmp - 1) / 2;
    idx_t prev = s * (s + 1) / 2;
    idx_t offset = n - prev;
    idx_t nn = offset;          /* row number in T */
    idx_t kk = s - offset;      /* column number */

    /* compute T(nn, kk) by the sum formula */
    seq_t sum = 0;
    for (idx_t j = 0; j <= nn; ++j) {
        seq_t bin1 = binomial((idx_t)((kk + 1) * nn), j);
        seq_t top2 = (seq_t)kk * nn - (seq_t)j - 1;
        seq_t bin2 = binom_general(top2, nn - j);
        sum += bin1 * bin2;
    }
    return sum;
}

/*
 * A382289  Irregular triangle T(n,k), n >= 0, 0 <= k <= 2*n+1, read by rows,
 *          where T(n,k) = [x^k] (1-x)^(n+1) * Sum_{k=0..n} (2*k+1)^n * x^k.
 * https://oeis.org/A382289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, easy
 * Formula:   * T(n,k) = Σ_{j=0}^{n} (-1)^{k−j} * (2j+1)^n * C(n+1, k−j) * (0
 *            ≤ k ≤ 2n+1) * * a(m) = T(r,c) where r = ⌊(√(4m+1)−1)/2⌋ and c =
 *            m − r(r+1) * (i.e. a(0)=T(0,0), a(1)=T(0,1), a(2)=T(1,0),
 *            a(3)=T(1,1), …)
 * Range:     i8 n <= 15, i16 n <= 25, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

/* integer square root of a 64‑bit unsigned integer */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* compute T(n,k) as defined by the formula above */
static inline seq_t triangle_T(idx_t n, idx_t k)
{
    idx_t n1 = n + 1;
    idx_t low  = (k > n1) ? (k - n1) : 0;
    idx_t high = (k < n)  ? k : n;

    seq_t sum = 0;

    for (idx_t j = low; j <= high; ++j) {
        /* (2j+1)^n */
        seq_t power = big_pow((seq_t)(2 * j + 1), (seq_t)n);

        /* sign (-1)^(k-j) */
        seq_t sign = ((k - j) & 1) ? (seq_t)-1 : (seq_t)1;

        /* C(n+1, k-j) */
        seq_t bin = binomial(n1, k - j);

        sum += sign * power * bin;
    }
    return sum;
}

/* public routine computing a(n) */
seq_t a(idx_t idx)
{
    /* find row r: r(r+1) ≤ idx < (r+1)(r+2) */
    idx_t tmp   = 1 + 4 * idx;
    idx_t sqr   = isqrt_u64(tmp);
    idx_t r     = (sqr - 1) >> 1;

    /* column within the row */
    idx_t c = idx - r * (r + 1);

    return triangle_T(r, c);
}

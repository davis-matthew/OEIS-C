/*
 * A244130  Triangle read by rows: coefficients T(n,k) of a binomial
 *          decomposition of n as Sum_{k=0..n} T(n,k)*binomial(n,k).
 * https://oeis.org/A244130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where * r = floor((sqrt(8n+1)-1)/2), * k = n -
 *            r(r+1)/2, * T(r,0) = 0, * T(r,k) = (-k)^(k-1) * (k+1)^(r-k) for
 *            k>0
 * Range:     i8 n <= 17, i16 n <= 33, i32 n <= 74, i64 n <= 168,
 *            i128 n <= 431, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res = res * base;
    return res;
}

seq_t a(idx_t n)
{
    idx_t r = 0;
    /* find the row r such that the cumulative number of elements up
       to row r exceeds n. */
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    idx_t prev = r * (r + 1) / 2;
    idx_t k = n - prev;

    if (k == 0)
        return (seq_t)0;

    seq_t kk = (seq_t)k;
    /* (-k)^(k-1) */
    seq_t part1 = pow_seq(-kk, k - 1);
    /* (k+1)^(r-k) */
    seq_t part2 = pow_seq(kk + 1, r - k);

    return part1 * part2;
}

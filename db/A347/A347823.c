/*
 * A347823  Triangle read by rows: T(n,k) = (n+k+1)*binomial(n,k), 0 <= k <=
 *          n.
 * https://oeis.org/A347823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (m + k + 1) * C(m, k), where n = m(m+1)/2 + k, 0 <= k <=
 *            m
 * Range:     u8 n <= 30, u16 n <= 111, u32 n <= 477, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root, floor of sqrt(x) for unsigned 64‑bit x */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = 1ULL << 62;               /* highest power of two <= 2^63 */
    while (bit != 0) {
        idx_t y = r | bit;
        if (y <= x / y) {
            r = y;
        }
        bit >>= 1;
    }
    return r;
}

/* binomial coefficient C(n,k) computed multiplicatively */
static inline seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n)         return 0;
    if (k > n - k)     k = n - k;     /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 0; i < k; ++i) {
        res = res * (seq_t)(n - i);
        res = res / (seq_t)(i + 1);
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find the row m such that m(m+1)/2 <= n < (m+1)(m+2)/2   */
    idx_t m = (isqrt_u64(1 + 8 * n) - 1) / 2;

    /* number of terms before row m */
    idx_t prev = m * (m + 1) / 2;

    /* position within the row */
    idx_t k = n - prev;

    /* value of the triangle entry */
    seq_t bin = binomial_small(m, k);
    return bin * (seq_t)(m + k + 1);
}

/*
 * A294579  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals, where
 *          A(n,k) is Sum_{d|n} d^(1 + k*n/d).
 * https://oeis.org/A294579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array * A(n,k) =
 *            Sum_{d|n} d^(1 + k*n/d) in antidiagonal order. * For a given
 *            index n, let t be the smallest integer with * t(t+1)/2 ≥ n. Then
 *            nRow = n - (t-1)t/2 and * kCol = t-1 - (n - (t-1)t/2 - 1). *
 *            a(n) = A(nRow,kCol)
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 125, u64 n <= 242,
 *            u128 n <= 479, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

/* integer square root using 128‑bit arithmetic */
static inline uint64_t isqrt_u64(uint64_t x)
{
    unsigned int b = 63;
    uint64_t res = 0, bit;
    for (; b >> 1; ) {
        bit = 1ULL << (b >> 1);
        if (res + bit <= x) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        b >>= 1;
    }
    return res;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* Find t such that t(t+1)/2 >= n */
    idx_t low = 1, high = 1;
    while (( (__uint128_t)high * (high + 1) ) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t val = ( (__uint128_t)mid * (mid + 1) ) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t t = low;

    /* previous triangular number */
    __uint128_t m_prev = ( (__uint128_t)(t - 1) * t ) / 2;
    idx_t m_prev_u = (idx_t)m_prev;

    /* position within the t-th antidiagonal */
    idx_t m = n - m_prev_u - 1;         /* 0‑based */
    idx_t nRow = m + 1;
    idx_t kCol = (t - 1) - m;

    /* sum over divisors of nRow */
    seq_t sum = (seq_t)0;
    idx_t NN = nRow;
    for (idx_t d = 1; d * d <= NN; ++d) {
        if (NN % d == 0) {
            idx_t d2 = NN / d;

            idx_t exp1 = 1 + kCol * d2;
            seq_t term1 = seq_pow((seq_t)d, exp1);
            sum += term1;

            if (d2 != d) {
                idx_t exp2 = 1 + kCol * d;
                seq_t term2 = seq_pow((seq_t)d2, exp2);
                sum += term2;
            }
        }
    }
    return sum;
}

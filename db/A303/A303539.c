/*
 * A303539  Number of ordered pairs (k, m) with 0 <= k <= m such that n -
 *          binomial(2*k,k) - binomial(2*m,m) can be written as the sum of two
 *          squares.
 * https://oeis.org/A303539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(k,m) : 0 ≤ k ≤ m, n - binom(2k,k) - binom(2m,m) can be
 *            written as a sum of two squares}
 * Range:     u8 n <= 443, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Integer square root for 64‑bit unsigned values. */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t r = (uint64_t)sqrtl((long double)x);
    while (((__uint128_t)(r + 1) * (r + 1)) <= x) ++r;
    while (((__uint128_t)r * r) > x) --r;
    return r;
}

/* Test whether a 64‑bit unsigned integer can be expressed as a sum of two squares. */
static inline bool is_sum_two_squares_uint64(uint64_t t)
{
    uint64_t limit = isqrt_uint64(t);
    for (uint64_t a = 0; a <= limit; ++a) {
        uint64_t a2 = a * a;
        uint64_t b2 = t - a2;
        uint64_t b  = isqrt_uint64(b2);
        if (b * b == b2) return true;
    }
    return false;
}

/* Compute A303539(n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined, but safe */

    /* Determine the largest m such that binom(2m,m) ≤ n. */
    const size_t MAXM = 128;                 /* generous upper bound */
    seq_t binom[MAXM];
    binom[0] = (seq_t)1;                     /* binom(0,0) = 1 */

    size_t m_max = 0;
    seq_t n_seq = (seq_t)n;
    for (size_t i = 1; i < MAXM; ++i) {
        /* recurrence: binom(2i,i) = binom(2i-2,i-1) * (4*i-2)/i */
        binom[i] = binom[i - 1] * ((seq_t)(4 * i - 2)) / (seq_t)i;
        if (binom[i] > n_seq) {
            m_max = i - 1;
            break;
        }
        if (i == MAXM - 1) m_max = i;        /* never reached for n ≤ 2^64 */
    }

    seq_t count = (seq_t)0;

    for (size_t m = 0; m <= m_max; ++m) {
        for (size_t k = 0; k <= m; ++k) {
            seq_t sum = binom[k] + binom[m];
            if (sum > n_seq) break;          /* binom[k] grows with k */
            seq_t rem = n_seq - sum;
            /* rem fits in 64 bits because n ≤ 2^64−1 and sum ≥ 0 */
            if (is_sum_two_squares_uint64((uint64_t)rem))
                ++count;
        }
    }

    return count;
}

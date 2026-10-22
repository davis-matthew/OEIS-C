/*
 * A324158  Expansion of Sum_{k>=1} x^k / (1 - k * x^k)^k.
 * https://oeis.org/A324158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^(d-1) * binomial(n/d + d - 2, d - 1)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 50, u64 n <= 107,
 *            u128 n <= 224, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer square root: floor(sqrt(n)) for 0 <= n < 2^64 */
static idx_t int_sqrt(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

/* Fast binary exponentiation */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result = result * base;
        exp >>= 1;
        if (exp) base = base * base;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* sequence is defined for n >= 1 */
    seq_t sum = 0;

    idx_t max_d = int_sqrt(n);
    for (idx_t d = 1; d <= max_d; ++d) {
        if (n % d != 0) continue;

        /* first divisor d */
        idx_t m_int = n / d;
        seq_t term = pow_seq((seq_t)m_int, d - 1);
        if (d > 1) {
            idx_t a = m_int + d - 2;
            idx_t k = d - 1;
            term = term * binomial(a, k);
        }
        sum += term;

        /* second divisor n/d, if distinct */
        idx_t d2 = n / d;
        if (d2 != d) {
            idx_t m2_int = n / d2;    /* which is d */
            seq_t term2 = pow_seq((seq_t)m2_int, d2 - 1);
            if (d2 > 1) {
                idx_t a2 = m2_int + d2 - 2;
                idx_t k2 = d2 - 1;
                term2 = term2 * binomial(a2, k2);
            }
            sum += term2;
        }
    }
    return sum;
}

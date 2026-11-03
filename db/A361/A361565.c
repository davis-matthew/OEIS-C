/*
 * A361565  a(n) is the numerator of the median of divisors of n.
 * https://oeis.org/A361565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator((A033676(n)+A033677(n))/2)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9565 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* safety for out‑of‑range input */

    seq_t sq = floor_sqrt((seq_t)n);  /* floor(sqrt(n)) */
    if (sq * sq == (seq_t)n) {
        /* n is a perfect square: the median divisor is sqrt(n) */
        return sq;
    }

    uint64_t nd = (uint64_t)n;
    uint64_t d  = (uint64_t)sq;       /* candidate divisor <= sqrt(n) */

    /* find the largest divisor <= sqrt(n) */
    while (d > 0) {
        if (nd % d == 0) break;
        d--;
    }

    seq_t d_seq  = (seq_t)d;
    seq_t other  = ((seq_t)n) / d_seq;   /* n / d_seq */
    seq_t sum    = d_seq + other;        /* sum of the two middle divisors */

    /* numerator of (sum)/2 in lowest terms: divide by 2 only if sum is even */
    if (sum % 2 == 0) {
        return sum / 2;
    } else {
        return sum;
    }
}

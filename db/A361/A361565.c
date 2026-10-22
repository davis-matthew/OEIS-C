/*
 * A361565  a(n) is the numerator of the median of divisors of n.
 * https://oeis.org/A361565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator((A033676(n)+A033677(n))/2)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

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

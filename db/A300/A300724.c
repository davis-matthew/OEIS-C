/*
 * A300724  Möbius transform of A053644(n), largest power of 2 less than or
 *          equal to n.
 * https://oeis.org/A300724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} μ(d) * 2^⌊log₂(n/d)⌋ Here μ(d) is the Möbius
 *            function, and 2^⌊log₂(n/d)⌋ is the largest power of two ≤ n/d
 *            (A053644). The implementation uses the provided `mobius` and
 *            `floor_log2_uint64` utilities
 * Range:     u8 n <= 511, u16 n <= 9530, u32 n <= 9530, u64 n <= 9530,
 *            u128 n <= 9530, big any n
 * Helpers:   floor_log2_uint64, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mobius.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n) {
    /* Sum over all divisors d of n. */
    seq_t sum = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* First divisor d1 = i */
            idx_t d1 = i;
            idx_t nd1 = n / d1;
            idx_t lg1 = floor_log2_uint64(nd1);
            seq_t pow1 = (seq_t)1 << lg1;          /* 2^lg1 */
            seq_t mu1  = mobius(d1);
            sum += mu1 * pow1;

            /* Second divisor d2 = n / i, avoid double count */
            idx_t d2 = n / i;
            if (d2 != d1) {
                idx_t nd2 = n / d2;                /* same as d1 */
                idx_t lg2 = floor_log2_uint64(nd2);
                seq_t pow2 = (seq_t)1 << lg2;      /* 2^lg2 */
                seq_t mu2  = mobius(d2);
                sum += mu2 * pow2;
            }
        }
    }
    return sum;
}

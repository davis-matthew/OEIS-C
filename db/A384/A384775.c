/*
 * A384775  Consecutive states of the linear congruential pseudo-random number
 *          generator 33952834046453*s mod 2^48 when started at 1.
 * https://oeis.org/A384775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 33952834046453 * a(n-1) mod 2^48 with a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t MULTIPLIER = (seq_t)33952834046453ULL;
static const seq_t MOD_MASK     = (seq_t)0xFFFFFFFFFFFFULL;  /* 2^48-1 */

seq_t a(idx_t n)
{
    if (n == 1U) return (seq_t)1;

    seq_t val = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        val = (val * MULTIPLIER) & MOD_MASK;
    }
    return val;
}

/*
 * A384779  Consecutive states of the linear congruential pseudo-random number
 *          generator 68909602460261*s mod 2^48 when started at 1.
 * https://oeis.org/A384779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 68909602460261 * a(n-1) mod 2^48, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MULTIPLIER ((seq_t)68909602460261ULL)
#define MOD_BITS 48
#define MOD_MASK ( ((seq_t)1 << MOD_BITS) - 1)

/* Compute base^exp mod 2^48 */
static inline seq_t pow_mod_48(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1)
            result = (result * b) & MOD_MASK;
        b = (b * b) & MOD_MASK;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                 /* undefined for n < 1, return 0 */
    return pow_mod_48(MULTIPLIER, n - 1);
}

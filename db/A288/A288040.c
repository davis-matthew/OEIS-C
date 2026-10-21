/*
 * A288040  Integers whose number of distinct decimal digits is prime.
 * https://oeis.org/A288040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate integers, count distinct decimal digits,
   accept if the count is prime (2,3,5,7). */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static const bool prime_digit_count[11] = {
    /* 0  1  2  3  4  5  6  7  8  9 10 */
    false, false, true, true, false, true, false, true, false, false, false
};

static inline int popcount10(unsigned int mask) {
    int c = 0;
    while (mask) {
        c += mask & 1u;
        mask >>= 1u;
    }
    return c;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1, but guard */
    idx_t found = 0;
    seq_t val = 10; /* a(1) = 10 */

    while (1) {
        /* Count distinct decimal digits of val */
        unsigned int mask = 0;
        seq_t tmp = val;
        while (tmp > 0) {
            unsigned int d = (unsigned int)(tmp % 10);
            mask |= 1u << d;
            tmp /= 10;
        }
        int bits = popcount10(mask);
        if (prime_digit_count[bits]) {
            ++found;
            if (found == n) {
                return val;
            }
        }
        ++val;
    }
}

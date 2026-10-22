/*
 * A357299  a(n) is the number of divisors of n whose first digit equals the
 *          first digit of n.
 * https://oeis.org/A357299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors d of n whose first digit equals first
 *            digit of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint32_t first_digit_uint64(uint64_t x) {
    while (x >= 10) x /= 10;
    return (uint32_t)x;
}

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    uint32_t fd_n = first_digit_uint64(nn);
    seq_t count = 0;

    /* Iterate over divisors up to sqrt(nn) */
    for (uint64_t i = 1; i <= nn / i; ++i) {
        if (nn % i == 0) {
            if (first_digit_uint64(i) == fd_n) ++count;

            uint64_t d2 = nn / i;
            if (d2 != i) {
                if (first_digit_uint64(d2) == fd_n) ++count;
            }
        }
    }
    return count;
}

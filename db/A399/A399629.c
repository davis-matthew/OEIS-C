/*
 * A399629  a(n) is the number of divisors d of n such that the first digit of
 *          d is equal to the last digit of n.
 * https://oeis.org/A399629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) = number of divisors d of n such that the first digit of d
 *            equals the last digit of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t first_digit(uint64_t x) {
    while (x >= 10) x /= 10;
    return x;
}

seq_t a(idx_t n) {
    /* n is positive (offset 1). */
    uint64_t last_digit = n % 10;
    seq_t count = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            if (first_digit(d1) == last_digit) ++count;

            idx_t d2 = n / i;
            if (d2 != d1 && first_digit(d2) == last_digit) ++count;
        }
    }
    return count;
}

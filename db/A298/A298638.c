/*
 * A298638  Numbers k such that the digital sum of k and the digital root of k
 *          have opposite parity.
 * https://oeis.org/A298638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the nth integer k such that digit_sum(k) and
 *            digital_root(k) have opposite parity
 * Range:     u8 n <= 122, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9928 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the parity (0 even, 1 odd) of the digital sum of an idx_t number. */
static unsigned parity_of_digit_sum(idx_t x) {
    unsigned sum = 0;
    while (x) {
        sum += (unsigned)(x % 10);
        x /= 10;
    }
    return sum & 1U;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 1;

    while (1) {
        unsigned ds = parity_of_digit_sum(k);
        /* Digital root parity: dr parity = 0 for even, 1 for odd. */
        unsigned dr = (k % 9 == 0) ? 1U : ((k % 9) & 1U);

        if (ds != dr) {
            ++count;
            if (count == n) {
                return (seq_t)(k);
            }
        }
        ++k;
    }
}

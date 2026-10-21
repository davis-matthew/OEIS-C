/*
 * A241146  Least number k such that k and n*k share at least one digit.
 * https://oeis.org/A241146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = least positive integer k such that the decimal
 *            representations of k and n*k share at least one digit
 * Range:     u8 n <= 31, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned digit_mask(seq_t x) {
    unsigned mask = 0;
    while (x) {
        unsigned d = (unsigned)(x % 10);
        mask |= 1u << d;
        x /= 10;
    }
    return mask;
}

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    for (seq_t k = 1; ; ++k) {
        unsigned mk = digit_mask(k);
        unsigned mnk = digit_mask(N * k);
        if (mk & mnk) {
            return k;
        }
    }
}

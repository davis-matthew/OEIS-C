/*
 * A347955  Dirichlet convolution of A003415 (arithmetic derivative) with
 *          A003602 (a Kimberling paraphrase).
 * https://oeis.org/A347955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A003415(n/d) * A003602(d)
 * Range:     u8 n <= 47, u16 n <= 2879, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 97 terms through n = 9660 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A003415_a(idx_t n) {
    if (n <= 1) return 0;
    seq_t result = 0;
    idx_t remaining = n;
    for (idx_t p = 2; p * p <= remaining; p++) {
        int exponent = 0;
        while (remaining % p == 0) {
            exponent++;
            remaining /= p;
        }
        if (exponent > 0) {
            result += (seq_t)(n / p) * (seq_t)exponent;
        }
    }
    if (remaining > 1) result += (seq_t)(n / remaining);
    return result;
}

static seq_t A003602_a(idx_t n) {
    while ((n & 1) == 0) n >>= 1;
    return (seq_t)((n + 1) >> 1);
}

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t d = 1; d <= n; d++) {
        if (n % d == 0) {
            total += A003415_a(n / d) * A003602_a(d);
        }
    }
    return total;
}

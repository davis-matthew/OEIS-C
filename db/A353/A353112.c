/*
 * A353112  Base-4 representation of A000422(n).
 * https://oeis.org/A353112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007090(A000422(n)); A000422(n) = n*10^len(A000422(n-1))
 *            + A000422(n-1), A000422(1) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A000422_a(idx_t n) {
    seq_t result = 1;
    seq_t power = 10;
    for (idx_t k = 2; k <= n; k++) {
        result = (seq_t)k * power + result;
        idx_t t = k;
        while (t > 0) {
            power *= 10;
            t /= 10;
        }
    }
    return result;
}

static seq_t A007090_a(seq_t m) {
    seq_t result = 0;
    seq_t power = 1;
    while (m > 0) {
        seq_t digit = m % 4;
        result += digit * power;
        power *= 10;
        m /= 4;
    }
    return result;
}

seq_t a(idx_t n) {
    return A007090_a(A000422_a(n));
}

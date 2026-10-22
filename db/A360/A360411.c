/*
 * A360411  Numbers k such that k*(k+1) does not contain the digit 2.
 * https://oeis.org/A360411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 307, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A360411: numbers k such that k*(k+1) has no digit 2 */

#include <stdbool.h>
#include <stdint.h>

static bool has_digit_two(seq_t x) {
    while (x > 0) {
        if (x % 10 == 2)
            return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 0;
    while (count < n) {
        ++k;
        seq_t prod = k * (k + 1);
        if (!has_digit_two(prod))
            ++count;
    }
    return k;
}

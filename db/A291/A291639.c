/*
 * A291639  Numbers k such that 0 is the smallest decimal digit of k^3.
 * https://oeis.org/A291639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer k such that k³ contains at
 *            least one decimal digit 0
 * Range:     u8 none, u16 n <= 8, u32 n <= 1817, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: return true if decimal representation of x contains a digit 0. */
static bool contains_zero(seq_t x) {
    while (x > 0) {
        if ((x % 10) == 0)
            return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 1;
    while (count < n) {
        seq_t cube = (seq_t)k * k * k;
        if (contains_zero(cube)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
    return 0; /* Unreachable – mask against compiler warnings. */
}

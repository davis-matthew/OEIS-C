/*
 * A253676  Irregular triangle T read by rows in which row n is the result of
 *          iterating the function S defined in A257480 and terminating at the
 *          first occurrence of 1, assuming the 3x+1 (or Collatz) conjecture.
 * https://oeis.org/A253676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   row n lists n, S(n), S(S(n)), ... down to the first 1, where S
 *            is A257480 (assumes the Collatz conjecture); a(n) is the n-th
 *            entry of the flattened rows, found by walking the rows
 * Range:     u8 n <= 42, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A253676_S(seq_t n) {
    seq_t x = (seq_t)4 * n - 3;
    seq_t t = (seq_t)3 * x + 1;
    while ((t & 1) == 0) t >>= 1;
    seq_t m = t + 1;
    seq_t p3 = 1;
    while ((m & 1) == 0) {
        m >>= 1;
        p3 *= 3;
    }
    return ((seq_t)3 + p3 * m) / 6;
}

seq_t a(idx_t n) {
    idx_t pos = n;
    idx_t r = 1;
    for (;;) {
        idx_t len = 1;
        seq_t x = (seq_t)r;
        while (x != 1) {
            x = A253676_S(x);
            ++len;
        }
        if (pos <= len) {
            x = (seq_t)r;
            idx_t k = pos - 1;
            while (k > 0) {
                x = A253676_S(x);
                --k;
            }
            return x;
        }
        pos -= len;
        ++r;
    }
}

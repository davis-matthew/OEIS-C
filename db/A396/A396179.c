/*
 * A396179  Positive integers not in A008726.
 * https://oeis.org/A396179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer not in A008726; computed blockwise:
 *            block q (q>=2) has 8 gaps of q-1 non-members, each right after
 *            4q(q-1)+j*q, j=0..7
 * Range:     u8 n <= 196, u16 n <= 5513, u32 n <= 5513, u64 n <= 5513,
 *            u128 n <= 5513, big any n
 * Validated: standard, pass, 95 terms through n = 5404 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t rem = (seq_t)n;
    seq_t q = 2;
    for (;;) {
        seq_t cnt = (seq_t)8 * (q - 1);
        if (rem <= cnt) {
            seq_t g = (rem - 1) / (q - 1);
            seq_t k = (rem - 1) % (q - 1);
            return (seq_t)4 * q * (q - 1) + g * q + 1 + k;
        }
        rem -= cnt;
        q++;
    }
}

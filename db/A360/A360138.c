/*
 * A360138  a(n) = 1 + A026430(A356133(n)).
 * https://oeis.org/A360138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + A026430(A356133(n)); A356133(n) = 3n - 1 - t(n-1),
 *            A026430(m) = m + #{k<m : t(k)=1}, t = Thue-Morse
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int tm_parity_seq(seq_t x)
{
    int p = 0;
    while (x > 0) {
        p ^= (int)(x & 1);
        x >>= 1;
    }
    return p;
}

seq_t a(idx_t n)
{
    int p = 0;
    idx_t k = n - 1;
    while (k > 0) {
        p ^= (int)(k & 1);
        k >>= 1;
    }
    seq_t m = (seq_t)3 * (seq_t)n - (seq_t)1 - (seq_t)p;   /* A356133(n) */
    seq_t ones;
    if ((m & 1) == 0) {
        ones = m / 2;
    } else {
        ones = (m - 1) / 2 + (seq_t)tm_parity_seq(m - 1);
    }
    return (seq_t)1 + m + ones;
}

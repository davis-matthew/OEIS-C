/*
 * A380544  Numbers of the form A073138(k) XOR A038573(k).
 * https://oeis.org/A380544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 0; for n >= 2, a(n) = (2^d - 2^(d-c)) XOR (2^c - 1), i.e.
 *            A073138(k) XOR A038573(k) for a k with d bits and c ones.
 *            Distinct values come from d >= 2, 1 <= c <= floor(d/2), listed
 *            in increasing order
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    idx_t m = n - 1;
    idx_t d = 2;
    while (m > d / 2) {
        m -= d / 2;
        ++d;
    }
    idx_t c = m;

    seq_t hi = ((seq_t)1 << d) - ((seq_t)1 << (d - c));
    seq_t lo = ((seq_t)1 << c) - (seq_t)1;
    return hi ^ lo;
}

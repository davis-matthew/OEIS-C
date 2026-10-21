/*
 * A258869  Expansion of 1 to the basis 1.880000478655... (A127583).
 * https://oeis.org/A258869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-7) for n >= 17; a(1)=1, a(2)=1, a(3)=1, a(4)=0,
 *            a(5)=0, a(6)=1, a(7)=0, a(8)=1, a(9)=1, a(10)=1, a(11)=0,
 *            a(12)=0, a(13)=1, a(14)=0, a(15)=1, a(16)=0 (Sequence A258869)
 *            The sequence becomes periodic with period 7 after the 16th term,
 *            so for n > 16 we can reduce the index modulo 7
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 1‑based table of the first 16 terms (index 0 unused) */
    static const seq_t table[17] = {
        0, /* unused */
        1, /* a(1) */
        1, /* a(2) */
        1, /* a(3) */
        0, /* a(4) */
        0, /* a(5) */
        1, /* a(6) */
        0, /* a(7) */
        1, /* a(8) */
        1, /* a(9) */
        1, /* a(10) */
        0, /* a(11) */
        0, /* a(12) */
        1, /* a(13) */
        0, /* a(14) */
        1, /* a(15) */
        0  /* a(16) */
    };

    if (n <= 16)
        return table[(size_t)n];

    /* For n > 16 use periodicity: a(n) = a(n-7) */
    while (n > 16)
        n -= 7U;

    return table[(size_t)n];
}

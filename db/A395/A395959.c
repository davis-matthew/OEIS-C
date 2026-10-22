/*
 * A395959  Numbers k such that A395958(k) = 0.
 * https://oeis.org/A395959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*(n-1) + A395958(2*(n-1))
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A395958(m) is the parity (0 or 1) of the bits of m whose positions satisfy (p & 2)==0
   (i.e. the lower two bits of each 4‑bit block). */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* sequence is defined for n ≥ 1, but this makes the function safe */
        return (seq_t)0;

    /* Base value 2*(n-1) computed in seq_t to avoid overflow of the shift on idx_t */
    seq_t base = ((seq_t)(n - 1)) << 1;

    /* m = 2*(n-1) computed in 64 bits for the parity test */
    uint64_t m = ((uint64_t)(n - 1)) << 1;

    /* Mask selecting positions where (p & 2) == 0 : bits 0,1,4,5,8,9,... */
    const uint64_t mask = 0x3333333333333333ULL;

    /* Parity of the selected bits */
    uint64_t sel = m & mask;
    unsigned parity = 0;          /* 0 or 1 */
    while (sel) {
        parity ^= 1;
        sel &= sel - 1;           /* turn off the lowest set bit */
    }

    return base + (seq_t)parity;
}

/*
 * A400471  Write the positive squares 1, 4, 9, 16, ... in base 2 and
 *          juxtapose.
 * https://oeis.org/A400471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy, new
 * Formula:   a(n) = the nth bit in the binary concatenation of positive
 *            squares (1, 4, 9, 16, ...)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* length in bits of an unsigned 64‑bit integer; works for 0..2^64-1 */
static unsigned bitlen_u64(uint64_t x)
{
    unsigned len = 0;
    while (x) {
        x >>= 1;
        len++;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* a(1) is the first bit of the first square */
    if (n < 1u)                  /* undefined for n = 0, but safe */
        return (seq_t)0;

    idx_t cumulative = 0;        /* total bits seen so far */

    for (idx_t s = 1; ; ++s) {
        /* compute s^2 in a type that cannot overflow for the ranges
           we expect to encounter (n up to at least 10⁴). */
        uint64_t sq = (uint64_t)s * (uint64_t)s;

        unsigned len = bitlen_u64(sq);          /* bits in s² */

        if (cumulative + len >= n) {
            /* position of the wanted bit within this square (1‑based) */
            unsigned pos = (unsigned)(n - cumulative);   /* 1 .. len */
            unsigned shift = len - pos;                  /* shift to LSB */
            return (seq_t)((sq >> shift) & 1ULL);
        }

        cumulative += len;                       /* move to next square */
    }
}

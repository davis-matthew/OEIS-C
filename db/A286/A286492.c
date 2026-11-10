/*
 * A286492  Positions of 1 in A286490; complement of A286491.
 * https://oeis.org/A286492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A286490, the
 *            {0101->0}-transform of the Thue-Morse word; computed by scanning
 *            the transformed word
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9553 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static inline int A286492_tm(idx_t i)
{
    idx_t v = i - 1;
    v ^= v >> 32;
    v ^= v >> 16;
    v ^= v >> 8;
    v ^= v >> 4;
    v ^= v >> 2;
    v ^= v >> 1;
    return (int)(v & 1U);
}

seq_t a(idx_t n)
{
    idx_t pos = 0;      /* output position */
    idx_t ones = 0;     /* ones seen so far */
    idx_t orig = 1;     /* position in Thue-Morse word */

    while (1) {
        int b0 = A286492_tm(orig);
        int out;
        if (b0 == 0 && A286492_tm(orig + 1) == 1 &&
            A286492_tm(orig + 2) == 0 && A286492_tm(orig + 3) == 1) {
            out = 0;
            orig += 4;
        } else {
            out = b0;
            orig += 1;
        }
        ++pos;
        if (out) {
            ++ones;
            if (ones == n)
                return (seq_t)pos;
        }
    }
}

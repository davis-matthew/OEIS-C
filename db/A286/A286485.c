/*
 * A286485  Positions of 0 in A286484; complement of A286486.
 * https://oeis.org/A286485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A286484, the
 *            {0010->0}-transform of the Thue-Morse word (scan left to right,
 *            replace 0010 by 0)
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9905 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned A286485_tm(idx_t i)
{
    i ^= i >> 32;
    i ^= i >> 16;
    i ^= i >> 8;
    i ^= i >> 4;
    i ^= i >> 2;
    i ^= i >> 1;
    return (unsigned)(i & 1);
}

seq_t a(idx_t n)
{
    idx_t pos = 0, out = 0, zeros = 0;
    if (n == 0) return (seq_t)0;
    for (;;) {
        unsigned b0 = A286485_tm(pos);
        unsigned sym;
        if (b0 == 0 && A286485_tm(pos + 1) == 0 && A286485_tm(pos + 2) == 1 && A286485_tm(pos + 3) == 0) {
            sym = 0;
            pos += 4;
        } else {
            sym = b0;
            pos += 1;
        }
        ++out;
        if (sym == 0) {
            ++zeros;
            if (zeros == n) return (seq_t)out;
        }
    }
}

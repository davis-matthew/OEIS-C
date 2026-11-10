/*
 * A285980  Positions of 1 in A285978; complement of A285979.
 * https://oeis.org/A285980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A285978, the {00->1} transform
 *            of the Thue-Morse word; computed by a single scan
 * Range:     u8 n <= 203, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9886 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned A285980_parity(uint64_t x)
{
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (unsigned)(x & 1);
}

seq_t a(idx_t n)
{
    idx_t pos = 0;   /* output positions produced so far */
    idx_t ones = 0;  /* ones found so far */
    uint64_t k = 0;

    while (1) {
        unsigned tk = A285980_parity(k);
        unsigned tn = A285980_parity(k + 1);
        unsigned bit;
        if (tk == 0 && tn == 0) {
            bit = 1;
            k += 2;
        } else {
            bit = tk;
            k += 1;
        }
        ++pos;
        if (bit) {
            ++ones;
            if (ones == n)
                return (seq_t)pos;
        }
    }
}

/*
 * A285979  Positions of 0 in A285978; complement of A285980.
 * https://oeis.org/A285979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285978, the {00->1}-transform
 *            of the Thue-Morse word A010060; computed by direct simulation
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9469 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint8_t A285979_parity(uint64_t x)
{
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (uint8_t)(x & 1);
}

seq_t a(idx_t n)
{
    uint64_t pos = 0;   /* output bits produced so far */
    uint64_t k = 0;     /* index in Thue-Morse */
    idx_t zeros = 0;

    while (1) {
        uint8_t tk = A285979_parity(k);
        uint8_t tn = A285979_parity(k + 1);
        ++pos;
        if (tk == 0 && tn == 0) {
            k += 2;          /* outputs a 1 */
        } else {
            if (tk == 0) {
                ++zeros;
                if (zeros == n)
                    return (seq_t)pos;
            }
            k += 1;
        }
    }
}

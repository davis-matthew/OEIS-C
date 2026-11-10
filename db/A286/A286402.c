/*
 * A286402  Positions of 1 in A286400; complement of A286401.
 * https://oeis.org/A286402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A286400, the
 *            {1010->1}-transform of the Thue-Morse word (computed by direct
 *            scan)
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9866 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline int A286402_par(uint64_t x)
{
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (int)(x & 1);
}

seq_t a(idx_t n)
{
    uint64_t i = 0;      /* index into Thue-Morse word */
    uint64_t out = 0;    /* output bits produced (1-based position of last) */
    uint64_t ones = 0;   /* ones found so far */
    uint64_t target = (uint64_t)n;

    if (target == 0) return (seq_t)0;

    while (ones < target) {
        int b0 = A286402_par(i);
        int bit;
        if (b0 && !A286402_par(i + 1) && A286402_par(i + 2) && !A286402_par(i + 3)) {
            bit = 1;
            i += 4;
        } else {
            bit = b0;
            i += 1;
        }
        out++;
        if (bit) ones++;
    }
    return (seq_t)out;
}

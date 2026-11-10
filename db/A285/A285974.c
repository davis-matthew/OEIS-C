/*
 * A285974  Positions of 1 in A285972; complement of A285973.
 * https://oeis.org/A285974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A285972 (the {10->1}-transform
 *            of Thue-Morse); computed by scanning the transformed word
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9987 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t i = 0;     /* index in Thue-Morse word */
    idx_t pos = 0;   /* output position (1-based after increment) */
    idx_t ones = 0;

    while (1) {
        int b1 = __builtin_parityll(i);
        int b2 = __builtin_parityll(i + 1);
        int bit;
        if (b1 == 1 && b2 == 0) {
            bit = 1;
            i += 2;
        } else {
            bit = b1;
            i += 1;
        }
        ++pos;
        if (bit) {
            ++ones;
            if (ones == n)
                return (seq_t)pos;
        }
    }
}

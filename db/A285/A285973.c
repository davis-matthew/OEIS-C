/*
 * A285973  Positions of 0 in A285972; complement of A285974.
 * https://oeis.org/A285973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285972, the {10->1}-transform
 *            of Thue-Morse; found by scanning the word
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9979 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t out = 0;
    idx_t zeros = 0;
    idx_t i = 0;
    for (;;) {
        int b1 = __builtin_parityll((unsigned long long)i);
        int b2 = __builtin_parityll((unsigned long long)(i + 1));
        ++out;
        if (b1 == 1 && b2 == 0) {
            i += 2;
        } else {
            if (b1 == 0) {
                ++zeros;
                if (zeros == n)
                    return (seq_t)out;
            }
            ++i;
        }
    }
}

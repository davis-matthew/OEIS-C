/*
 * A284684  Positions of 0 in A284627; complement of A284685.
 * https://oeis.org/A284684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A284627 (scan of the Thue-Morse
 *            word with 0011 -> 1)
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9543 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t i = 0;      /* index in Thue-Morse word */
    uint64_t pos = 0;    /* output position (1-based after increment) */
    idx_t zeros = 0;

    while (1) {
        int p0 = __builtin_parityll(i);
        if (p0 == 0 &&
            __builtin_parityll(i + 1) == 0 &&
            __builtin_parityll(i + 2) == 1 &&
            __builtin_parityll(i + 3) == 1) {
            /* 0011 -> single 1 */
            ++pos;
            i += 4;
            continue;
        }
        ++pos;
        if (p0 == 0) {
            ++zeros;
            if (zeros == n)
                return (seq_t)pos;
        }
        ++i;
    }
}

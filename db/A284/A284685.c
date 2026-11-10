/*
 * A284685  Positions of 1 in A284627; complement of A284684.
 * https://oeis.org/A284685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A284627, the Thue-Morse word
 *            with each 0011 replaced by 1 (scan, count 1s)
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 8401 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t i = 0;      /* index in Thue-Morse word */
    idx_t out = 0;    /* position in transformed word (1-based after increment) */
    idx_t cnt = 0;    /* ones seen */

    while (1) {
        int sym;
        int p0 = __builtin_parityll(i);
        if (p0 == 0 &&
            __builtin_parityll(i + 1) == 0 &&
            __builtin_parityll(i + 2) == 1 &&
            __builtin_parityll(i + 3) == 1) {
            sym = 1;
            i += 4;
        } else {
            sym = p0;
            ++i;
        }
        ++out;
        if (sym) {
            ++cnt;
            if (cnt == n)
                return (seq_t)out;
        }
    }
}

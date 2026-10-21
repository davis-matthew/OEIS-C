/*
 * A276137  Numbers without the decimal digits 2, 4, 6 and 8.
 * https://oeis.org/A276137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the number whose decimal digits correspond to the base‑6
 *            representation of (n‑1), with digits 0→0, 1→1, 2→3, 3→5, 4→7,
 *            5→9. Thus a(1)=0, a(2)=1, a(3)=3, a(4)=5, …
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; compute base‑6 digits of n‑1 */
    idx_t m = n - 1;
    uint8_t digits[64];
    idx_t pos = 0;
    do {
        digits[pos++] = m % 6;
        m /= 6;
    } while (m > 0);

    static const seq_t map[6] = {(seq_t)0, (seq_t)1, (seq_t)3,
                                 (seq_t)5, (seq_t)7, (seq_t)9};

    seq_t result = 0;
    for (int i = (int)pos - 1; i >= 0; --i) {
        result = result * 10 + map[digits[i]];
    }
    return result;
}

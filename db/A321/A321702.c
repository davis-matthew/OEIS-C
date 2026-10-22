/*
 * A321702  Numbers that are still valid after a horizontal reflection on a
 *          calculator display.
 * https://oeis.org/A321702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers that stay valid after a horizontal reflection on a
 *            calculator display are those whose decimal digits belong to the
 *            set {0,1,2,3,5,8}. The n‑th term (a(1)=0) can be obtained by
 *            writing n-1 in base 6, then replacing the base‑6 digits
 *            0,1,2,3,4,5 by 0,1,2,3,5,8 respectively and interpreting...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned char map_digits[6] = {0, 1, 2, 3, 5, 8};

seq_t a(idx_t n)
{
    /* Convert n-1 to base 6, map digits, then build the base‑10 number.  */
    idx_t m = n - 1;                 /* n is at least 1 */
    unsigned char digits[32];        /* enough for all 64‑bit indices */
    int    pos = 0;

    /* Decompose m into base‑6 digits, storing the mapped decimal digit */
    while (m > 0) {
        unsigned int d = (unsigned int)(m % 6);
        digits[pos++] = map_digits[d];
        m = m / 6;
    }

    if (pos == 0)                    /* n == 1  → 0 */
        return 0;

    seq_t result = 0;
    for (int i = pos - 1; i >= 0; --i)
        result = result * 10 + digits[i];

    return result;
}

/*
 * A293873  Numbers having '13' as substring of their digits.
 * https://oeis.org/A293873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth integer whose decimal representation contains the
 *            substring "13"
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether a decimal integer contains the consecutive digits '1''3'. */
static bool contains13(seq_t x)
{
    /* Keep checking while x has at least two digits. */
    while (x >= 13)
    {
        unsigned d1 = (unsigned)(x % 10u);       /* least significant digit   */
        unsigned d2 = (unsigned)((x / 10u) % 10u);/* next digit to the left     */
        if (d1 == 3u && d2 == 1u)
            return true;
        x /= 10u;
    }
    return false;
}

/* Return the nth term of the sequence. n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0u)
        return 0u;                       /* undefined, but safe */

    idx_t found = 0u;
    seq_t num = 13u;                    /* first candidate containing "13" */

    while (found < n)
    {
        if (contains13(num))
        {
            ++found;
            if (found == n)
                return num;
        }
        ++num;
    }
    return 0u;                           /* unreachable, keeps function total */
}

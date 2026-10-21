/*
 * A294568  Decimal expansion of 1/18779.
 * https://oeis.org/A294568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 104, u64 n <= 104, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: long division to obtain the n‑th decimal digit of 1/18779. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Using long division: repeatedly multiply remainder by 10, then
       divide by 18779 to extract each decimal digit. */
    seq_t rem = 1;           /* initial remainder: 1 in 1/18779 */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= (seq_t)10;           /* bring next decimal place */
        digit = rem / (seq_t)18779; /* next digit */
        rem   = rem % (seq_t)18779; /* new remainder for next step */
    }
    return digit; /* digit is between 0 and 9 */
}

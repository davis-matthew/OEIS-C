/*
 * A284891  Concatenation of the numbers from 3 to n.
 * https://oeis.org/A284891
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = concat(3..n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0; /* Undefined for n < 3, but return 0 for safety */

    seq_t result = 0;
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t part = (seq_t)i;          /* current integer to append   */
        idx_t d = digits(i);            /* number of decimal digits of i */
        result = result * pow10(d) + part;
    }
    return result;
}

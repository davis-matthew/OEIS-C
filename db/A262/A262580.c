/*
 * A262580  Concatenation of the numbers from 1 to n but omitting 10.
 * https://oeis.org/A262580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A262580: concatenation of the positive integers 1,2,3,…,
   skipping the integer 10.  For each n ≥ 1, a(n) is the decimal
   number obtained by writing the first n terms of this list consecutively. */

#include <stdbool.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t result = 0;
    idx_t count  = 0;   /* how many terms have been added */
    idx_t i      = 1;   /* current integer to consider */

    while (count < n)
    {
        if (i == 10)    /* skip the number 10 */
        {
            ++i;
            continue;
        }

        /* factor = 10 ^ digits(i)  (digits(i) is the number of decimal digits of i) */
        seq_t factor = pow10((idx_t)digits(i));
        result = result * factor + i;  /* append i to the right */
        ++count;
        ++i;
    }

    return result;
}

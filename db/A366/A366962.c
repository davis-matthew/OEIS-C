/*
 * A366962  Numbers whose difference between the largest and smallest digits
 *          is equal to 5.
 * https://oeis.org/A366962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose difference between the largest and smallest digits
 *            is equal to 5
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of numbers found so far */
    seq_t x = 0;              /* current tested number */

    while (1)
    {
        x += 1;               /* move to next integer */

        /* Find smallest and largest decimal digits of x */
        seq_t temp = x;
        unsigned int min_d = 9;
        unsigned int max_d = 0;
        while (temp > 0)
        {
            unsigned int d = (unsigned int)(temp % 10);
            if (d < min_d) min_d = d;
            if (d > max_d) max_d = d;
            temp /= 10;
        }

        /* Check the required property */
        if (max_d - min_d == 5)
        {
            count++;
            if (count == n)
                return x;
        }
    }
}

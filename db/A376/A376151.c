/*
 * A376151  TARDIS numbers: the sum of the first and last digits of the
 *          decimal expansion is less than the sum of the middle digits.
 * https://oeis.org/A376151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* TARDIS numbers – brute‑force search for numbers where  
   sum(first + last digits) < sum(middle digits)      */
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t   count = 0;
    seq_t   num   = 120;   /* first TARDIS number */

    while (1) {
        /* Sum of all digits */
        seq_t temp   = num;
        unsigned int sum = 0;
        while (temp > 0) {
            sum += (unsigned int)(temp % 10);
            temp /= 10;
        }

        /* First digit */
        seq_t t = num;
        while (t >= 10) t /= 10;
        unsigned int first = (unsigned int)(t);

        /* Last digit */
        unsigned int last = (unsigned int)(num % 10);

        unsigned int middle = sum - first - last;

        if (first + last < middle) {
            ++count;
            if (count == n) return num;
        }
        ++num;
    }
}

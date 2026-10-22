/*
 * A366959  Numbers whose difference between the largest and smallest digits
 *          is equal to 2.
 * https://oeis.org/A366959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer where (maximum digit – minimum digit) == 2
 * Range:     u8 n <= 34, u16 n <= 1358, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 0, but safe */

    idx_t count = 0;                 /* how many valid terms seen so far */
    idx_t num   = 0;                 /* current integer being tested */

    while (count < n) {
        ++num;                       /* advance to next integer */

        /* examine digits of num without overflowing seq_t */
        idx_t      x   = num;
        unsigned char min_d = 9;
        unsigned char max_d = 0;

        while (x > 0) {
            unsigned char d = x % 10;
            if (d < min_d) min_d = d;
            if (d > max_d) max_d = d;
            x /= 10;
        }

        if (max_d - min_d == 2) {
            ++count;                 /* valid term found */
        }
    }

    return (seq_t)num;               /* n-th term (may truncate if seq_t too small) */
}

/*
 * A248521  Numbers k such that floor(cot(Pi/(2k+2))) = floor(cot(Pi/(2k))) +
 *          1.
 * https://oeis.org/A248521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth positive integer k satisfying *
 *            floor(cot(Pi/(2k+2))) = floor(cot(Pi/(2k))) + 1 * (OEIS A248521)
 * Range:     u8 n <= 162, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Pi as a long double for good precision */
    static const long double PI = acosl(-1.0L);

    idx_t seen = 0;          /* how many members of the sequence we have found */
    idx_t k = 1;             /* current integer to test for membership */

    while (1)
    {
        /* compute floor(cot(Pi/(2k+2))) */
        long double angle_left  = PI / (2.0L * (long double)(k + 1));
        long double cot_left    = 1.0L / tanl(angle_left);
        int64_t   floor_left    = (int64_t)floorl(cot_left + 1e-15L);

        /* compute floor(cot(Pi/(2k))) + 1 */
        long double angle_right = PI / (2.0L * (long double)k);
        long double cot_right   = 1.0L / tanl(angle_right);
        int64_t   floor_right   = (int64_t)floorl(cot_right + 1e-15L) + 1;

        /* test the defining condition */
        if (floor_left == floor_right)
        {
            ++seen;
            if (seen == n)
                return (seq_t)k;   /* the n-th member */
        }

        ++k;   /* try the next integer */
    }
}

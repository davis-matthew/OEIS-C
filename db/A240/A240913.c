/*
 * A240913  Numbers m such that GCD of digits of m is 1 and no digit of m is
 *          1.
 * https://oeis.org/A240913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Sequential search for integers whose digits have GCD 1
   and contain no digit 1 */
#include <stdbool.h>

/* integer gcd for small digit values */
static int gcd_int(int a, int b)
{
    while (b != 0)
    {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* test whether a number satisfies the conditions */
static bool valid(idx_t m)
{
    int gcd = 0;
    while (m != 0)
    {
        int d = m % 10;          /* get least significant decimal digit */
        if (d == 1)              /* digit '1' is forbidden */
            return false;
        if (gcd == 0)
            gcd = d;
        else
            gcd = gcd_int(gcd, d);
        m /= 10;
    }
    return gcd == 1;
}

/* public sequence function */
seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of found terms so far */
    idx_t m = 2;              /* candidate numbers start from 2 */

    while (1)
    {
        if (valid(m))
        {
            count++;
            if (count == n)
                return (seq_t)m;
        }
        m++;
    }
}

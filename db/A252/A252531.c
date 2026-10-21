/*
 * A252531  Number of (n+2) X (7+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 2 3 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-3) for n>5 Initial values: a(1)=1200, a(2)=1468,
 *            a(3)=2404, a(4)=3160, a(5)=11456
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute 8^exp as a seq_t. */
static seq_t pow8(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)8;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Offset is 1, so a(1) is 1200.  n is always >= 1. */
    switch (n)
    {
        case 1: return (seq_t)1200;
        case 2: return (seq_t)1468;
        case 3: return (seq_t)2404;
        case 4: return (seq_t)3160;
        case 5: return (seq_t)11456;
        default:
            /* For n >= 6 use the recurrence a(n) = 8 * a(n-3). */
            {
                idx_t k = (n - 3) / 3;   /* power of 8 needed */
                idx_t r = n % 3;         /* determines which base value to use */

                seq_t base;
                if (r == 0)
                    base = (seq_t)2404;   /* a(3) */
                else if (r == 1)
                    base = (seq_t)3160;   /* a(4) */
                else
                    base = (seq_t)11456;  /* a(5) */

                return base * pow8(k);
            }
    }
}

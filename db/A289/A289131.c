/*
 * A289131  a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3) - 2*a(n-4) + 2*a(n-5) for n >=
 *          6, a(0) = 2, a(1) = 4, a(2) = 7, a(3) = 11, a(4) = 18, a(5) = 28.
 * https://oeis.org/A289131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2; for n>0: * if n is even: a(n)=7*2^(n/2)-4-(3*n)/2; * if
 *            n is odd : a(n)=(5*2^((n+3)/2)-9-3*n)/2. * Recurrence: a(n) =
 *            a(n-1)+3*a(n-2)-3*a(n-3)-2*a(n-4)+2*a(n-5) for n≥6 * (used only
 *            for validation)
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 58, u64 n <= 122, u128 n <= 250,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 2;

    if ((n & 1) == 0)   /* even n */
    {
        seq_t p = pow2(n / 2);          /* 2^(n/2) */
        seq_t val = p * 7;              /* 7*2^(n/2) */
        val -= 4;                       /* minus 4 */
        seq_t sub = (seq_t)n * 3 / 2;   /* (3*n)/2 */
        val -= sub;                     /* subtract (3*n)/2 */
        return val;
    }
    else                 /* odd n */
    {
        seq_t p = pow2((n + 3) / 2);    /* 2^((n+3)/2) */
        seq_t val = p * 5;              /* 5*2^((n+3)/2) */
        val -= 9;                       /* minus 9 */
        val -= (seq_t)n * 3;            /* minus 3*n */
        return val / 2;                 /* divide by 2 */
    }
}

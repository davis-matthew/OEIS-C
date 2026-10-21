/*
 * A241205  Sum of x*y^2*z^3 for positive integers x,y,z with x + y + z = n.
 * https://oeis.org/A241205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n-1)*(n-2)*(n+2)*(n+1)*(3*n^2+1)/10080
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 44, u64 n <= 706,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;                 /* n=1,2 give 0 because (n-2)=0 */

    unsigned __int128 tmp = 1;

    /* multiply all factors of the numerator */
    tmp *= (unsigned __int128)n;         /* n          */
    tmp *= (unsigned __int128)n;         /* n          */
    tmp *= (unsigned __int128)(n-1);     /* (n-1)      */
    tmp *= (unsigned __int128)(n-2);     /* (n-2)      */
    tmp *= (unsigned __int128)(n+1);     /* (n+1)      */
    tmp *= (unsigned __int128)(n+2);     /* (n+2)      */

    /* compute 3*n^2 + 1 in 128‑bit */
    unsigned __int128 n2   = (unsigned __int128)n * n;   /* n^2      */
    unsigned __int128 extra = (unsigned __int128)(3 * n2 + 1);  /* 3*n^2+1 */

    tmp *= extra;                      /* multiply by 3*n^2+1 */

    /* divide by the constant denominator 10080 */
    tmp /= 10080;

    return (seq_t)tmp;                  /* cast back to the build’s seq_t */
}

/*
 * A269403  Expansion of x*(2 - x + 2*x^2 + x^3)/((1 - x)^3*(1 + x + x^2 +
 *          x^3)).
 * https://oeis.org/A269403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 + 2*n + 2*sin((Pi*n)/2) - (-1)^n + 1)/4
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The formula is evaluated using integer arithmetic only.        */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to signed long long for intermediate calculation */
    long long n_ll = (long long)n;

    /* Compute sin(Pi*n/2) as an integer pattern: 0,1,0,-1 repeated. */
    long long s;
    switch ((int)(n & 3))   /* n % 4 */
    {
        case 0: s = 0; break;
        case 1: s = 1; break;
        case 2: s = 0; break;
        case 3: s = -1; break;
    }

    /* (-1)^n: 1 if n even, -1 if n odd */
    long long sign_even = (n & 1) ? -1 : 1;

    /* Compute numerator: 2*n^2 + 2*n + 2*s + 1 - sign_even */
    long long numerator = 2 * n_ll * n_ll + 2 * n_ll + 2 * s + 1 - sign_even;

    /* Division by 4 yields an integer result */
    long long result = numerator / 4;

    return (seq_t)result;
}

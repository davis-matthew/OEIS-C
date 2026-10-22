/*
 * A397513  a(n) = Sum_{k=0..n} binomial(5*n+2*k-3,k).
 * https://oeis.org/A397513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(5*n+2*k-3, k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Term for k = 0 is 1 */
    seq_t sum  = 1;
    seq_t term = 1;

    seq_t five_n = (seq_t)5 * n;     /* 5*n, used repeatedly */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Numerators and denominators for the recurrence
           T_k = T_{k-1} * (5n+2k-4)(5n+2k-3) / (k*(5n+k-3))      */
        seq_t numer1 = five_n + (seq_t)2 * k - 4;
        seq_t numer2 = five_n + (seq_t)2 * k - 3;
        seq_t den1   = k;
        seq_t den2   = five_n + k - 3;

        /* First multiply by numer1 and divide by den1 */
        seq_t g = gcd(numer1, den1);
        numer1 /= g;
        den1   /= g;

        g = gcd(term, den1);
        term /= g;
        den1  /= g;                     /* den1 should become 1 */

        seq_t tmp = term * numer1;      /* exact division */
        tmp = tmp / den1;               /* just to keep the same pattern */

        /* Now multiply by numer2 and divide by den2 */
        g = gcd(numer2, den2);
        numer2 /= g;
        den2   /= g;

        g = gcd(tmp, den2);
        tmp  /= g;
        den2  /= g;                     /* den2 should become 1 */

        term = tmp * numer2;
        term = term / den2;              /* division is by 1 */

        /* Add to cumulative sum */
        sum += term;
    }

    return sum;
}

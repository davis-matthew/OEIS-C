/*
 * A262343  Numerator of 3*(1-2/n), for n >= 3.
 * https://oeis.org/A262343
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator(3*(1-2/n)) = 3*(n-2)/gcd(3*(n-2),n) (for n ≥ 3)
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned long long gcd_ull(unsigned long long a, unsigned long long b)
{
    while (b != 0) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                           /* behaviour below the offset is irrelevant */

    unsigned long long N   = n;                 /* use a wide type to avoid overflow */
    unsigned long long num = 3ULL * (N - 2ULL); /* numerator of 3*(n-2)      */

    unsigned long long g   = gcd_ull(num, N);   /* gcd(3*(n-2), n)            */
    unsigned long long res = num / g;           /* reduced numerator          */

    return (seq_t)res;                          /* cast to return type         */
}

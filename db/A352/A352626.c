/*
 * A352626  a(n) = (n+1)*(3*n-2)*C(2*n,n-1)/(4*n-2).
 * https://oeis.org/A352626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(3*n-2)*C(2*n,n-1)/(4*n-2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* denominator first */
    seq_t denom = 4 * (seq_t)n - 2;

    /* initial value: binomial(2n, n-1) */
    seq_t num = binomial(2 * n, n - 1);

    /* cancel common factors with the denominator */
    seq_t g = gcd(num, denom);
    num /= g;
    denom /= g;

    /* multiply by (n+1) */
    seq_t factor = (seq_t)n + 1;
    num *= factor;
    g = gcd(num, denom);
    num /= g;
    denom /= g;

    /* multiply by (3*n-2) */
    factor = 3 * (seq_t)n - 2;
    num *= factor;
    g = gcd(num, denom);
    num /= g;
    denom /= g;

    /* At this point denom should be 1; return the result */
    return num;
}

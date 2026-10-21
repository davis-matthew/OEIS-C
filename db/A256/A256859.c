/*
 * A256859  a(n) = n*(n + 1)*(n + 2)*(n^2 - n + 4)/24.
 * https://oeis.org/A256859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n*n - n + 4)/24
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 159, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t denom  = 24;
    seq_t term;
    seq_t g;

    /* factor n */
    term = (seq_t)n;
    g = gcd(denom, term);
    term /= g;
    denom /= g;
    result *= term;

    /* factor n+1 */
    term = (seq_t)n + 1;
    g = gcd(denom, term);
    term /= g;
    denom /= g;
    result *= term;

    /* factor n+2 */
    term = (seq_t)n + 2;
    g = gcd(denom, term);
    term /= g;
    denom /= g;
    result *= term;

    /* factor n^2 - n + 4 */
    term = (seq_t)n * (seq_t)n - (seq_t)n + 4;
    g = gcd(denom, term);
    term /= g;
    denom /= g;
    result *= term;

    /* any remaining denominator (should be 1) */
    return result;
}

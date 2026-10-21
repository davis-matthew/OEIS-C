/*
 * A268579  Expansion of (1 + 6*x + x^2 + 12*x^3 - 2*x^4)/((1 - x)^4*(1 +
 *          x)^3).
 * https://oeis.org/A268579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + ( n*(6*n^2 + 27*n + 35) - (9*n^2 + 15*n + 2)*(-1)^n +
 *            2 ) / 16 Computation uses 64‑bit unsigned intermediates to avoid
 *            overflow even for very small seq_t types. The result is then
 *            cast back to seq_t
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to 64‑bit unsigned for intermediate calculations */
    unsigned long long tn = (unsigned long long)n;
    unsigned long long n2 = tn * tn;                     /* n^2                */
    unsigned long long term1 = tn * (6 * n2 + 27 * tn + 35); /* n*(6n^2+27n+35)   */
    unsigned long long term2 = 9 * n2 + 15 * tn + 2;          /* 9n^2+15n+2        */
    unsigned long long numerator;

    if ((n & 1ULL) == 0)                     /* even n */
        numerator = term1 + 2 - term2;
    else                                    /* odd n  */
        numerator = term1 + term2 + 2;

    return ((seq_t)(numerator / 16)) + 1;
}

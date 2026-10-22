/*
 * A374084  a(n) = (1 + (n+1)^2 + (n-2)*(-1)^n)/2.
 * https://oeis.org/A374084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + (n+1)^2 + (n-2)*(-1)^n)/2
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a direct closed form to compute
 * the sequence efficiently for all n ≥ 1.  The calculation is
 * done in a signed intermediate type (long long) that is
 * sufficient for the problem domain (n up to a few 10⁴),
 * then cast back to seq_t.  All arithmetic uses only
 * integer operators (+, -, *, /). */
seq_t a(idx_t n)
{
    /* Convert n to a signed integer for the computation */
    long long nn = (long long)n;
    /* Compute (-1)^n as +1 for even n and -1 for odd n */
    long long sign = (n & 1) ? -1LL : 1LL;
    /* Compute the numerator of the formula */
    long long term = 1LL + (nn + 1LL) * (nn + 1LL) + (nn - 2LL) * sign;
    /* Divide by 2 to get the final value */
    return (seq_t)(term / 2LL);
}

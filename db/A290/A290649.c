/*
 * A290649  The largest number z less than or equal to 3n+1 such that
 *          binomial(z,n) is odd.
 * https://oeis.org/A290649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 For n > 0, write n = 2^e + d with 0 <= d < 2^e. Then
 *            a(n) = min( 3·2^e + a(d), 2^{e+2} – 1 ) (This gives the largest
 *            z ≤ 3n + 1 such that binomial(z, n) is odd.)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 7;

    /* Find the greatest power of two ≤ n and its exponent e */
    idx_t pow2 = 1;
    idx_t e = 0;
    while ((pow2 << 1) <= n) {
        pow2 <<= 1;
        ++e;
    }

    idx_t d = n - pow2;

    /* Compute both possibilities of the recurrence */
    seq_t val1 = ((seq_t)3 * pow2) + a(d);
    seq_t val2 = ((seq_t)pow2 << 2) - 1;   /* 2^{e+2} – 1 */

    return (val1 < val2) ? val1 : val2;
}

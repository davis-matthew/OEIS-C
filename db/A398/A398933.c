/*
 * A398933  Partial sums of A398929.
 * https://oeis.org/A398933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 + (3*q^2 - q)/2 + q*r + (2*n+1 - (-1)^n)/4
 *            where q = floor(n/3) and r = n - 3*q
 * Range:     u8 n <= 18, u16 n <= 312, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic */
    seq_t ns = (seq_t)n;

    /* Term 1: n*(n+1)/2 */
    seq_t term1 = ns * (ns + 1) / 2;

    /* Compute q and r */
    idx_t q = n / 3;
    idx_t r = n - 3 * q;

    /* Convert q and r to seq_t */
    seq_t qs = (seq_t)q;
    seq_t rs = (seq_t)r;

    /* Term 2: (3*q^2 - q)/2 = q*(3*q - 1)/2 */
    seq_t temp = qs * 3 - 1;          /* 3*q - 1   */
    seq_t term2 = qs * temp / 2;      /* q*(3*q-1)/2 */

    /* Term 3: q*r */
    seq_t term3 = qs * rs;

    /* Term 4: (2*n+1 - (-1)^n)/4
       For n even: n/2
       For n odd:  (n+1)/2
       So equivalent to (n/2) + (n mod 2) */
    seq_t term4 = (ns / 2) + (seq_t)(n & 1);

    /* Sum all terms */
    return term1 + term2 + term3 + term4;
}

/*
 * A259108  a(n) = 2 * A000538(n).
 * https://oeis.org/A259108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15*n^4 + 10*n^3 + 6*n^5 - n)/15
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Polynomial form derived from a(n) = -n/15 + (2*n^3)/3 + n^4 + (2*n^5)/5 */

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type to avoid intermediate overflow */
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;        /* n^2   */
    seq_t n3 = n2 * N;       /* n^3   */
    seq_t n4 = n3 * N;       /* n^4   */
    seq_t n5 = n4 * N;       /* n^5   */

    /* Compute the numerator of the rational formula:
       15 * a(n) = 15*n^4 + 10*n^3 + 6*n^5 - n
       All intermediate terms fit within seq_t for the ranges considered. */
    seq_t numer = 6 * n5 + 15 * n4 + 10 * n3 - N;

    /* Final result is integer as the numerator is divisible by 15 */
    return numer / 15;
}

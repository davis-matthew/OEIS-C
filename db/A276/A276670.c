/*
 * A276670  Numerator of (n-1)*n*(n+1)/4.
 * https://oeis.org/A276670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)*n*(n+1))/4 if n is odd or n % 4 == 0,
 *            ((n-1)*n*(n+1))/2 otherwise (this gives the numerator of
 *            ((n-1)*n*(n+1))/4 after cancelling common factors with the
 *            denominator 4)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (n-1)*n*(n+1) as seq_t */
    seq_t a = (seq_t)n;
    seq_t b = a - 1;          /* (n-1), wraps to MAX when n==0 */
    seq_t c = a + 1;          /* (n+1) */
    seq_t v = a * b * c;      /* product, fits in seq_t for n <= 1e9 */

    /* Decide whether to divide by 4 or 2 based on n modulo 4 */
    if ((n & 1ULL) || (n % 4ULL) == 0ULL)
        return v / 4;
    else
        return v / 2;
}

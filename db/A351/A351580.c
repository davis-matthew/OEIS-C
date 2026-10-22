/*
 * A351580  a(n) is the number of multisets of size n-1 consisting of
 *          permutations of n elements.
 * https://oeis.org/A351580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n! + n - 2, n - 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 1 */
    if (n <= 1)
        return (seq_t)1;

    /* First compute n! */
    seq_t f = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        f *= (seq_t)i;

    /* Compute the binomial coefficient multiplicatively:
       C(f + n - 2, n - 1) = Π_{i=1}^{n-1} (f + n - 1 - i) / i
    */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n - 1; ++i)
    {
        seq_t numerator = f + (seq_t)((n - 1) - i); /* f + (n-1-i) */
        result *= numerator;
        result /= (seq_t)i; /* exact division */
    }

    return result;
}

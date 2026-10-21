/*
 * A253945  a(n) = 6*binomial(n+1,5).
 * https://oeis.org/A253945
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*binomial(n+1,5) = ((n+1)*n*(n-1)*(n-2)*(n-3))/20
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 153, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;           /* undefined for n < 4 */
    /* Compute the product (n+1)*n*(n-1)*(n-2)*(n-3) as seq_t */
    seq_t prod = (seq_t)n + 1;
    prod *= (seq_t)n;
    prod *= (seq_t)(n - 1);
    prod *= (seq_t)(n - 2);
    prod *= (seq_t)(n - 3);
    /* Divide exactly by 20 ( = 2^2 * 5 ); the result is an integer */
    prod /= (seq_t)20;
    return prod;
}

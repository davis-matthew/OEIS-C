/*
 * A349088  a(n) = n! * Sum_{k=0..floor((n-1)/3)} 1 / (3*k+1)!.
 * https://oeis.org/A349088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor((n-1)/3)} 1 / (3*k+1)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is defined as 0. */
    if (n == 0)
        return (seq_t)0;

    /* Number of summands in the definition. */
    idx_t max_k = (n - 1) / 3;

    /* term_k = product of integers from (3*k+2) up to n.
       For k = 0 this is the product 2*3*...*n = n!.
       For subsequent k we can obtain the next term by dividing
       by the three integers that were removed. */
    seq_t term = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        term *= (seq_t)i;          /* term for k = 0 */

    seq_t sum = term;           /* add first summand */

    for (idx_t k = 1; k <= max_k; ++k)
    {
        /* Divide by the three integers that no longer appear
           in the product for the next k. */
        seq_t d1 = (seq_t)(3 * k - 1);
        seq_t d2 = (seq_t)(3 * k);
        seq_t d3 = (seq_t)(3 * k + 1);
        term /= (d1 * d2 * d3);
        sum += term;
    }

    return sum;
}

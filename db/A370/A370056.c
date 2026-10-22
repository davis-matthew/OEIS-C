/*
 * A370056  a(n) = 2*(4*n+1)!/(3*n+2)!.
 * https://oeis.org/A370056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (4*n+1)! / ( (3*n+2)! ) (for n >= 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special‑case n == 0:  2 * 1! / 2!  = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Compute the product (4*n+1)! / (3*n+2)! as a simple
       multiplicative product over the range [3*n+3 , 4*n+1]. */
    seq_t prod = 1;
    seq_t start = (seq_t)(3 * n + 3);
    seq_t end   = (seq_t)(4 * n + 1);

    for (seq_t k = start; k <= end; ++k)
        prod *= k;

    /* Multiply by the leading 2 in the definition. */
    return prod * 2;
}

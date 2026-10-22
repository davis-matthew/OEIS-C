/*
 * A321243  a(n) is the product of n and all its decimal digits individually
 *          except the leftmost digit.
 * https://oeis.org/A321243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * product of the decimal digits of n except the
 *            leftmost digit
 * Range:     u8 n <= 28, u16 n <= 898, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* a(0) = 0 */

    /* Find the leftmost (most significant) decimal digit of n */
    idx_t leftmost = n;
    while (leftmost >= 10)
        leftmost /= 10;

    /* Compute the product of all decimal digits of n */
    seq_t prod_all = 1;
    idx_t tmp = n;
    while (tmp > 0) {
        prod_all *= (seq_t)(tmp % 10);
        tmp /= 10;
    }

    /* If any digit is zero, the product is zero */
    if (prod_all == 0)
        return 0;

    /* Product of digits except the leftmost digit */
    seq_t prod_except = prod_all / (seq_t)leftmost;

    /* Final result */
    return (seq_t)n * prod_except;
}

/*
 * A333321  a(n) is the number of subsets of {1..n} that contain exactly 1 odd
 *          and 4 even numbers.
 * https://oeis.org/A333321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n/2) * binomial(floor(n/2), 4)
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 321, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 8 the count is zero since we need 4 even numbers. */
    if (n < 8)
        return 0;

    /* ceil(n/2) -> number of odd integers in 1..n */
    seq_t odd  = ((seq_t)n + 1) / 2;
    /* floor(n/2) -> number of even integers in 1..n */
    seq_t even = (seq_t)n / 2;

    /* If there are fewer than 4 even numbers, the binomial is zero. */
    if (even < 4)
        return 0;

    /* Compute binomial(even,4) = even*(even-1)*(even-2)*(even-3)/24. */
    seq_t bin = even * (even - 1);
    bin *= (even - 2);
    bin *= (even - 3);
    bin /= 24;

    return odd * bin;
}

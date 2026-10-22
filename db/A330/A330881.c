/*
 * A330881  Length of longest LB factorization over all binary strings of
 *          length n.
 * https://oeis.org/A330881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(8n+i) = 6n+i for 0 ≤ i ≤ 5, and a(8n+6) = a(8n+7) = 6n+5
 * Range:     u8 n <= 339, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute quotient and remainder of n divided by 8 */
    idx_t q = n / 8;        /* floor(n/8)  */
    idx_t r = n % 8;        /* n mod 8     */

    /* Apply the piecewise linear rule */
    if (r <= 5)
        return (seq_t)q * 6 + r;
    else
        return (seq_t)q * 6 + 5;
}

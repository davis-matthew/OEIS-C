/*
 * A268945  Number of length-4 0..n arrays with no repeated value unequal to
 *          the previous repeated value plus one mod n+1.
 * https://oeis.org/A268945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 4*n^3 + 3*n^2 + n + 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast index to the sequence type for accurate arithmetic. */
    seq_t x = (seq_t)n;

    /* Compute powers of x. */
    seq_t x2 = x * x;          /* x^2 */
    seq_t x3 = x2 * x;         /* x^3 */
    seq_t x4 = x3 * x;         /* x^4 */

    /* Apply the polynomial. */
    return x4 + (seq_t)4 * x3 + (seq_t)3 * x2 + x + (seq_t)1;
}

/*
 * A277985  a(n) = 3*(9*n - 1)*(3*n - 2).
 * https://oeis.org/A277985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(9*n - 1)*(3*n - 2)
 * Range:     u8 n <= 2, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to the sequence type to avoid intermediate truncation */
    seq_t nn = (seq_t)n;
    /* Compute the factors separately for clarity and potential
       arbitrary‑precision support.  All operations are in seq_t. */
    seq_t term1 = (seq_t)9 * nn - 1;   /* 9*n - 1 */
    seq_t term2 = (seq_t)3 * nn - 2;   /* 3*n - 2 */
    return (seq_t)3 * term1 * term2;
}

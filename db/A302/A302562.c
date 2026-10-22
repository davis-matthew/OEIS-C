/*
 * A302562  Partial sums of A092181.
 * https://oeis.org/A302562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7 - 10*n^2 + 15*n^3 + 18*n^4)/30
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t to avoid integer promotion issues */
    seq_t nn = (seq_t)n;

    /* compute successive powers of n */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */

    /* compute the polynomial expression */
    seq_t expr = (seq_t)7
                 - (seq_t)10 * n2
                 + (seq_t)15 * n3
                 + (seq_t)18 * n4;

    /* multiply by n and divide by 30 (exact division) */
    seq_t num = nn * expr;
    return num / (seq_t)30;
}

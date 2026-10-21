/*
 * A274250  Number of partitions of n^2 into at most three parts.
 * https://oeis.org/A274250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((n^2+3)^2/12)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 354, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^2 as a seq_t to avoid intermediate overflow. */
    seq_t n2 = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t t   = n2 + 3;                      /* n^2 + 3 */
    seq_t num = t * t;                       /* (n^2 + 3)^2 */
    /* Add 6 (i.e., 12/2) before dividing to implement rounding to nearest integer. */
    seq_t res = (num + 6) / 12;
    return res;
}

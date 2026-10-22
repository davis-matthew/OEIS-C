/*
 * A304611  a(n) = 155*n - 38.
 * https://oeis.org/A304611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 155*n - 38
 * Range:     u8 n <= 1, u16 n <= 423, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 */

/* Compute the nth term of sequence A304611.
 * a(1) = 117, a(2) = 272, etc.  Formula: a(n) = 155*n - 38.
 */
seq_t a(idx_t n)
{
    /* Convert the index to seq_t before arithmetic to avoid
       unintended truncation or overflow in the multiplication. */
    return ( (seq_t)n * (seq_t)155 ) - (seq_t)38;
}

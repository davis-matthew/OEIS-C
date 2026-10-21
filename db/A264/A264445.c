/*
 * A264445  a(n) = n*(n + 11)*(n + 22)/6.
 * https://oeis.org/A264445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+11)*(n+22)/6
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t before arithmetic to ensure proper type */
    seq_t x = (seq_t)n;
    return x * (x + 11) * (x + 22) / 6;
}

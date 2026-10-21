/*
 * A271027  a(n) = 3661529 + 11184810*n.
 * https://oeis.org/A271027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3661529 + 11184810*n
 * Range:     u8 none, u16 none, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast constants and index to seq_t to avoid mixed-type arithmetic */
    seq_t base   = (seq_t)3661529;
    seq_t coeff  = (seq_t)11184810;
    /* compute the value */
    return base + coeff * (seq_t)n;
}

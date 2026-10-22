/*
 * A329502  Expansion of (1+x)*(1+2*x)/(1-x).
 * https://oeis.org/A329502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n==0 ? 1 : (n==1 ? 4 : 6))
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329502: expansion of (1+x)*(1+2*x)/(1-x) */
seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;          /* a(0) = 1 */
    else if (n == 1U)
        return (seq_t)4;          /* a(1) = 4 */
    else
        return (seq_t)6;          /* a(n) = 6 for n >= 2 */
}

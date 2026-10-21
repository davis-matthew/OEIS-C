/*
 * A275736  a(n) has base-2 representation with ones in those digit-positions
 *          where n contains ones in its factorial base representation, and
 *          zeros in all the other positions.
 * https://oeis.org/A275736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i≥0} [d_i(n) == 1] * 2^i where d_i(n) is the i‑th
 *            digit (0‑based, least significant first) of the factorial base
 *            representation of n
 * Range:     u8 n <= 9617, u16 n <= 9617, u32 n <= 9617, u64 n <= 9617,
 *            u128 n <= 9617, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t m   = n;
    unsigned int bit = 0;       /* position in the binary result   */
    for (idx_t divisor = 1; m > 0; ++divisor, ++bit) {
        idx_t digit = m % (divisor + 1);   /* factorial digit  */
        if (digit == 1)
            res |= ((seq_t)1 << bit);      /* set the corresponding binary bit */
        m /= (divisor + 1);               /* move to the next higher position   */
    }
    return res;
}

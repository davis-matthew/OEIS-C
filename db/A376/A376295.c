/*
 * A376295  The binary expansion of a(n) is the reversal of the concatenation
 *          of the binary expansions of 1,...,n.
 * https://oeis.org/A376295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is obtained by concatenating the binary
   representations of 1,2,…,n, reversing this bit string,
   and interpreting the result as a binary integer. */
seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = n; i > 0; --i) {
        idx_t x = i;
        while (x > 0) {
            idx_t bit = x & 1;           /* LSB of x */
            res = (res << 1) | (seq_t)bit;
            x >>= 1;
        }
    }
    return res;
}

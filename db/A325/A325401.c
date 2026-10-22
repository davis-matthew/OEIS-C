/*
 * A325401  minflip(n) = min(n, r(n)) where r(n) is the binary reverse of n.
 * https://oeis.org/A325401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = min(n, A030101(n))
 * Range:     u8 n <= 255, u16 n <= 9955, u32 n <= 9955, u64 n <= 9955,
 *            u128 n <= 9955, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the binary reverse of n */
    seq_t rev = 0;
    idx_t temp = n;
    while (temp) {
        rev = (rev << 1) | (temp & 1);
        temp >>= 1;
    }

    seq_t n_seq = (seq_t)n;
    return (rev < n_seq) ? rev : n_seq;
}

/*
 * A325402  maxflip(n) = max(n, r(n)) where r(n) is the binary reverse of n.
 * https://oeis.org/A325402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = max(n, r(n)) where r(n) is the binary reverse of n
 * Range:     u8 n <= 255, u16 n <= 9765, u32 n <= 9765, u64 n <= 9765,
 *            u128 n <= 9765, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t rev_bits(idx_t n)
{
    seq_t rev = 0;
    while (n) {
        rev = (rev << 1) | (n & 1);
        n >>= 1;
    }
    return rev;
}

seq_t a(idx_t n)
{
    seq_t n_seq = n;
    seq_t rn = rev_bits(n);
    return (n_seq > rn) ? n_seq : rn;
}

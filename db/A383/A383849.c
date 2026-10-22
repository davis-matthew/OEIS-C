/*
 * A383849  In the binary representation of n, rotate right by the number of
 *          ones.
 * https://oeis.org/A383849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = rotation of binary representation of n to the right by
 *            popcount(n) bits (within its bit length)
 * Range:     u8 n <= 256, u16 n <= 9556, u32 n <= 9556, u64 n <= 9556,
 *            u128 n <= 9556, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t popcount_idx(idx_t x)
{
    idx_t c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
}

static inline idx_t bitlen_idx(idx_t x)
{
    idx_t l = 0;
    while (x) {
        ++l;
        x >>= 1;
    }
    return l;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t len   = bitlen_idx(n);
    idx_t ones  = popcount_idx(n);
    idx_t k     = ones % len;

    if (k == 0)
        return (seq_t)n;

    seq_t num   = (seq_t)n;
    idx_t shift = len - k;
    seq_t rot   = (num >> k) | (num << shift);

    /* Mask to keep only the low 'len' bits.  If 'len' equals the bit
       width of the type, masking is unnecessary because the left shift
       never produces bits beyond the type width. */
    if (len == sizeof(seq_t) * 8)
        return rot;

    seq_t mask = ((seq_t)1 << len) - 1;
    return rot & mask;
}

/*
 * A245788  n times the number of 1's in the binary expansion of n.
 * https://oeis.org/A245788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * popcount(n) (popcount = number of 1's in binary
 *            representation of n)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t popcount_idx(idx_t x)
{
    idx_t c = 0;
    while (x) {
        x &= x - 1;  /* drop lowest set bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* multiplication is done in seq_t to avoid overflow of idx_t */
    return (seq_t)n * (seq_t)popcount_idx(n);
}

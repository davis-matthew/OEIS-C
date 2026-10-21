/*
 * A279521  Maximum number of single-direction edges in leveled binary trees
 *          with n nodes.
 * https://oeis.org/A279521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 2^(h-1) - 1)*He + (-1)^He * (2^h - 1) where h =
 *            floor(log2(n)), He = H(-n + 3·2^(h-1) - 1). This simplifies to:
 *            a(n) = n - 2^(h-1) if n <= 3·2^(h-1) - 1 a(n) = 2^h - 1
 *            otherwise with a(1) = 0
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Compute h = floor(log2(n)) */
    idx_t h = 0;
    idx_t tmp = n;
    while (tmp >>= 1)
        ++h;

    /* 2^(h-1) and 2^h as unsigned 64-bit values */
    idx_t two_h_minus1 = 1ULL << (h - 1);
    idx_t two_h        = two_h_minus1 << 1;

    /* He is 1 iff n <= 3*2^(h-1) - 1 */
    idx_t limit = 3 * two_h_minus1 - 1;

    if (n <= limit)
        return (seq_t)(n - two_h_minus1);
    else
        return (seq_t)(two_h - 1);
}

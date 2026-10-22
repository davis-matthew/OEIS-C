/*
 * A353292  a(n) is the number of positive integers k <= n that have at least
 *          one common 1-bit with n.
 * https://oeis.org/A353292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + 1 - 2^{#zero bits below the highest set bit of n}
 * Range:     u8 n <= 270, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* work in 64-bit to compute the bit pattern */
    uint64_t nn = (uint64_t)n;

    /* position of the most significant 1-bit (0‑indexed) */
    unsigned int pos = 63 - __builtin_clzll(nn);

    /* mask for bits below the highest 1-bit */
    uint64_t lower_mask = ((uint64_t)1 << pos) - 1;
    uint64_t lower = nn & lower_mask;

    /* number of zero bits among the lower bits */
    unsigned int zero_bits = pos - __builtin_popcountll(lower);

    /* count of all non‑negative k ≤ n with no common 1‑bits to n */
    seq_t count_all = ((seq_t)1) << zero_bits;

    /* a(n) = n - (count_all - 1) = n + 1 - count_all */
    return (seq_t)n + 1 - count_all;
}

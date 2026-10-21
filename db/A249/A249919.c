/*
 * A249919  Number of LCD (liquid-crystal display) segments needed to display
 *          n in binary.
 * https://oeis.org/A249919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(0) = 6; for n > 0, a(n) = 6*bitlen(n) - 4*popcount(n)
 * Range:     u8 n <= 9520, u16 n <= 9520, u32 n <= 9520, u64 n <= 9520,
 *            u128 n <= 9520, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where bitlen(n) is the number of bits in the binary representation of n (no leading zeros) */

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 explicitly */
    if (n == 0)
        return (seq_t)6;

    /* Count the number of bits in n */
    idx_t bits = 0;
    idx_t tmp = n;
    while (tmp) {
        bits++;
        tmp >>= 1;
    }

    /* Count the number of 1‑bits in n */
    idx_t ones = (idx_t)__builtin_popcountll((unsigned long long)n);

    /* Compute the segment count */
    return (seq_t)6 * bits - (seq_t)4 * ones;
}

/*
 * A375183  In the binary expansion of n, complement bits at even positions
 *          and to the right of a 1 (the most significant bit corresponding to
 *          position 1).
 * https://oeis.org/A375183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary string of n with every even‑indexed bit flipped *
 *            if the preceding (more significant) bit is 1. The most
 *            significant * position is 1 (odd). Example: n = 9 (1001₂) → a(9)
 *            = 1101₂ = 13
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0) return 0;

    /* compute the bit length of n (msb position = 1) */
    idx_t tmp = n;
    idx_t bitlen = 0;
    while (tmp)
    {
        bitlen++;
        tmp >>= 1;
    }

    /* initialise result with the most significant bit */
    idx_t prev_bit_orig = (n >> (bitlen - 1)) & 1;
    seq_t res = (seq_t)prev_bit_orig;

    /* process remaining bits from msb to lsb */
    for (idx_t i = 2; i <= bitlen; i++)
    {
        idx_t bit_i_orig = (n >> (bitlen - i)) & 1;
        seq_t res_bit;

        if (((i & 1) == 0) && (prev_bit_orig == 1))
            res_bit = (seq_t)(1 - bit_i_orig);
        else
            res_bit = (seq_t)bit_i_orig;

        res = (res << 1) | res_bit;
        prev_bit_orig = bit_i_orig;
    }

    return res;
}

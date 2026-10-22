/*
 * A371442  For any positive integer n with binary digits (b_1, ..., b_w)
 *          (where b_1 = 1), the binary digits of a(n) are (b_1, b_3, ...,
 *          b_{2*ceiling(w/2)-1}); a(0) = 0.
 * https://oeis.org/A371442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   keep the odd‑indexed bits of n’s binary representation (index 1
 *            = most significant bit)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 0;
    /* number of bits in n (w ≥ 1) */
    int w = 64 - __builtin_clzll((uint64_t)n);

    /* i = 0 … w‑1 correspond to b1, b2, … , bw */
    for (int i = 0; i < w; ++i)
    {
        uint64_t bit = (n >> (w - 1 - i)) & 1ULL;
        /* keep bits with odd 1‑based index → i even */
        if ((i & 1) == 0)
            result = (result << 1) | (seq_t)bit;
    }
    return result;
}

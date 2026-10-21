/*
 * A269723  Start with A_0 = 0, then extend by setting B_k = complement of A_k
 *          and A_{k+1} = A_k A_k B_k B_k; sequence is limit of A_k as k ->
 *          infinity.
 * https://oeis.org/A269723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A010060(n) XOR A341389(n)
 * Range:     u8 n <= 9977, u16 n <= 9977, u32 n <= 9977, u64 n <= 9977,
 *            u128 n <= 9977, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the parity of the number of 1‐bits in the odd‑index positions of n. */

seq_t a(idx_t n)
{
    seq_t parity = 0;
    idx_t tmp    = n;
    idx_t bitpos = 0;
    while (tmp != 0) {
        if (tmp & 1) {
            if (bitpos & 1)
                parity ^= 1;
        }
        bitpos++;
        tmp >>= 1;
    }
    return parity;
}

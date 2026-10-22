/*
 * A375156  In the binary expansion of n: expand bits 1 -> 01 and 0 -> x0 from
 *          most to least significant, where x is the complement of the
 *          previous bit from n.
 * https://oeis.org/A375156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion: a(n) is obtained by expanding the binary representation of n.
   Starting from the most significant bit, each 1 is replaced by the two‑bit sequence 0,1.
   Each 0 is replaced by the two‑bit sequence x,0 where x is the complement of the
   previous bit of n (the previous bit for the first original bit is considered 0).
   For n=0, the single bit 0 produces the two‑bit sequence 1,0, i.e. decimal 2.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;                 // special case: 0 -> "10"

    seq_t res = (seq_t)0;
    uint64_t prev = 0;                   // previous bit of n (0 for first)
    /* determine bit length of n (i.e., position of highest set bit) */
    int bitlen = 0;
    for (idx_t tmp = n; tmp; tmp >>= 1)
        ++bitlen;

    for (int i = bitlen - 1; i >= 0; --i) {
        uint64_t bit = (n >> i) & 1ULL;
        if (bit == 1) {
            // expand 1 -> 0,1
            res = (res << 1) | (seq_t)0;
            res = (res << 1) | (seq_t)1;
        } else {
            // expand 0 -> x,0 where x = complement of previous bit
            uint64_t x = prev ^ 1ULL;
            res = (res << 1) | (seq_t)x;
            res = (res << 1) | (seq_t)0;
        }
        prev = bit;
    }
    return res;
}

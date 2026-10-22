/*
 * A341335  For any number n with binary expansion (b_1, ..., b_k), the binary
 *          expansion of a(n), say (c_1, ..., c_k) satisfies c_m = Sum_{d | m}
 *          b_d mod 2 for m = 1..k.
 * https://oeis.org/A341335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   For n with binary expansion (b1,…,bk) where b1 is the
 *            most‑significant bit, let (c1,…,ck) be the binary expansion of
 *            a(n) defined by c_m ≡ Σ_{d|m} b_d (mod 2) for all 1 ≤ m ≤ k.
 *            Example: a(2)=3, a(3)=2, a(4)=7
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* number of bits (length of binary representation) */
    idx_t k = 0;
    idx_t tmp = n;
    while (tmp) { ++k; tmp >>= 1; }

    seq_t res = 0;

    /* process each set bit of n; p counts from 0 (LSB) upwards */
    for (idx_t p = 0; p < k; ++p) {
        if ((n >> p) & 1) {            /* bit at position p is set */
            idx_t d = k - p;           /* position index starting at 1 from MSB */
            for (idx_t m = d; m <= k; m += d) {
                idx_t dest = k - m;    /* corresponding bit position from LSB */
                res ^= ((seq_t)1 << dest);
            }
        }
    }
    return res;
}

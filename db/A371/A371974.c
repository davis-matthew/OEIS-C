/*
 * A371974  For any positive integer n with binary expansion (b_1, ..., b_w)
 *          (where b_1 = 1), the binary expansion of a(n) is (c_1, ..., c_w)
 *          with c_k = (Sum_{i = 1 mod (w+1-k)} b_i) mod 2 for k = 1..w; a(0)
 *          = 0.
 * https://oeis.org/A371974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = transform of the binary expansion of n as defined for
 *            OEIS sequence A371974: c_k = (Σ_{i≡1 (mod w+1−k)} b_i) mod 2
 *            where b_1…b_w are the bits of n (b_1 is the most significant
 *            bit)
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Determine number of bits w of n (since n>0, w≥1). */
    idx_t w = 0;
    idx_t tmp = n;
    while (tmp) {
        w++;
        tmp >>= 1;
    }

    /* Store bits b_1 … b_w (1‑based).  b_1 is most significant bit. */
    unsigned int bits[65];
    for (idx_t i = 0; i < w; ++i) {
        bits[i + 1] = (unsigned int)((n >> (w - 1 - i)) & 1ULL);
    }

    seq_t ans = 0;
    /* Compute c_k for k = 1 .. w, accumulating msb first. */
    for (idx_t k = 1; k <= w; ++k) {
        idx_t d = w + 1 - k;          /* stride for indices. */
        unsigned int parity = 0;
        for (idx_t idx = 1; idx <= w; idx += d) {
            parity ^= bits[idx];
        }
        ans = (ans << 1) | (seq_t)parity;
    }
    return ans;
}

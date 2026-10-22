/*
 * A371959  For any positive integer n with binary expansion (b_1, ..., b_w)
 *          (where b_1 = 1), the binary expansion of a(n) is (c_1, ..., c_w)
 *          with c_k = (Sum_{i = 1..ceiling(k/2)} b_i * b_{k + 1 - i}) mod 2
 *          for any k = 1..w; a(0) = 0.
 * https://oeis.org/A371959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the binary convolution of n’s binary digits with itself:
 *            c_k = (sum_{i=1}^{ceil(k/2)} b_i * b_{k+1-i}) mod 2 for k = 1 ..
 *            w, where w = floor(log2(n)) + 1 and b_1 ... b_w is the binary
 *            representation of n with b_1 = 1
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Determine width w = number of bits in n */
    idx_t w = 0;
    idx_t tmp = n;
    while (tmp) {
        ++w;
        tmp >>= 1;
    }

    /* Extract the bits of n into an array (MSB first) */
    bool bits[64];
    for (idx_t i = 0; i < w; ++i) {
        /* shift by (w-1-i) so that i=0 gives the most significant bit */
        bits[i] = (bool)((n >> (w - 1 - i)) & 1ull);
    }

    /* Compute each output bit c_k and build the result */
    seq_t result = 0;
    for (idx_t k = 1; k <= w; ++k) {
        bool parity = false;
        idx_t half = (k + 1) / 2;              /* ceil(k/2) */
        for (idx_t t = 0; t < half; ++t) {
            idx_t idx1 = t;                    /* i-1    */
            idx_t idx2 = k - 1 - t;           /* k-i-1  */
            if (bits[idx1] && bits[idx2])
                parity ^= true;               /* toggle parity */
        }
        result = (result << 1) | (seq_t)parity;
    }
    return result;
}

/*
 * A342697  For any number n with binary expansion Sum_{k >= 0} b(k) * 2^k,
 *          the binary expansion of a(n) is Sum_{k >= 0} floor((b(k) + b(k+1)
 *          + b(k+2))/2) * 2^k.
 * https://oeis.org/A342697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = Σ_k floor((b_k + b_{k+1} + b_{k+2}) / 2) * 2^k where n =
 *            Σ_k b_k * 2^k (binary expansion of n)
 * Range:     u8 n <= 767, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int getbit(idx_t n, unsigned k) {
    /* return bit k of n (0 if k>=64) */
    return k >= 64 ? 0 : (int)((n >> k) & 1U);
}

seq_t a(idx_t n) {
    /* Determine bit length of n */
    unsigned bitlen = 0;
    idx_t temp = n;
    while (temp) {
        temp >>= 1;
        ++bitlen;
    }

    /* Output bits up to (bitlen + 2) – beyond that all contributions are 0 */
    const unsigned limit = bitlen + 2;
    seq_t res = 0;

    /* Width of seq_t in bits */
    const unsigned seq_width = sizeof(seq_t) * 8U;

    for (unsigned k = 0; k < limit; ++k) {
        int b0 = getbit(n, k);
        int b1 = getbit(n, k + 1);
        int b2 = getbit(n, k + 2);
        int sum = b0 + b1 + b2;
        if (sum >= 2) {
            if (k < seq_width)
                res |= ((seq_t)1 << k);
            /* If k >= seq_width we cannot set the bit in res; its value
               would not fit in seq_t, which is acceptable because such a
               value cannot be represented in this build. */
        }
    }

    return res;
}

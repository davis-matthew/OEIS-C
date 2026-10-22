/*
 * A361942  For any number n >= 0 with binary expansion (b_1, ..., b_w), a(n)
 *          is the least p > 0 such that b_i = b_{p+i} for i = 1..w-p.
 * https://oeis.org/A361942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal period of the binary expansion of n (ignoring
 *            leading zeros)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* by definition a(0)=1 */

    /* bit length w of n */
    idx_t w = 0;
    idx_t tmp = n;
    while (tmp) { ++w; tmp >>= 1; }

    /* brute‑force search for the smallest period p */
    for (idx_t p = 1; p <= w; ++p) {
        unsigned char ok = 1;
        for (idx_t i = 0; i + p < w; ++i) {
            /* bits at positions i and i+p (0 = MSB) */
            idx_t bit_i  = (n >> (w - 1 - i)) & 1;
            idx_t bit_ip = (n >> (w - 1 - (i + p))) & 1;
            if (bit_i != bit_ip) { ok = 0; break; }
        }
        if (ok) return (seq_t)p;
    }

    /* p = w always satisfies the condition; this line should never be reached */
    return (seq_t)w;
}

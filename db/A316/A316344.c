/*
 * A316344  An example of a word that is uniform morphic, but neither pure
 *          morphic, primitive morphic, nor recurrent.
 * https://oeis.org/A316344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A010060(2*n+2) + A010060(max(2*n+1,4)) A010060(k) is the
 *            parity of the binary representation of k (0 if k has an even
 *            number of 1-bits, 1 otherwise)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t parity_idx(idx_t x) {
    seq_t r = 0;
    while (x) {
        r ^= (seq_t)(x & 1);
        x >>= 1;
    }
    return r;
}

seq_t a(idx_t n) {
    /* parity of n */
    seq_t p_n = parity_idx(n);

    /* parity of n+1 (note the possible overflow of 64‑bit addition) */
    seq_t p_n1;
    if (n == UINT64_MAX) {
        /* n+1 = 1 << 64, which has a single 1‑bit */
        p_n1 = 1;
    } else {
        p_n1 = parity_idx(n + 1);
    }

    /* second term: parity(2*n+1) is parity(n)+1 for n>=2,
       otherwise it equals parity(4)=1 for n=0,1. */
    seq_t second = (n <= 1) ? 1 : (p_n ^ 1);

    return p_n1 + second;
}

/*
 * A327195  For any n >= 0: consider the different ways to split the binary
 *          representation of n into two (possibly empty) parts, say with
 *          value x and y; a(n) is the least possible value of abs(x^2 - y^2).
 * https://oeis.org/A327195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 2297, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A327195: a(n) = min_{0≤i≤L} |x² – y²|,  
    where L is the bit‑length of n and
    (x, y) are the values of the binary prefix and suffix obtained by
    splitting the binary representation of n at position i. */
#include <stdint.h>
#include <stdbool.h>

/* bit length of a zero‑based unsigned 64‑bit integer (0 → 0) */
static inline unsigned bitlen_idx(idx_t v) {
    if (v == 0) return 0;
    return 64 - __builtin_clzll((unsigned long long)v);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* a(0) = 0 */

    unsigned L = bitlen_idx(n);            /* number of bits in n */
    seq_t best = 0;
    unsigned first = 1;                    /* lazy init of best */

    for (unsigned i = 0; i <= L; ++i) {
        unsigned shift = L - i;            /* bits to right shift x */

        /* compute prefix x */
        idx_t x_idx;
        if (shift == 64)                  /* n is 64 bits, i == 0 */
            x_idx = 0;
        else
            x_idx = n >> shift;

        /* compute suffix y */
        idx_t y_idx;
        if (shift == 64)                  /* i == 0, suffix is all bits */
            y_idx = n;
        else if (shift == 0)              /* suffix empty */
            y_idx = 0;
        else
            y_idx = n & (((idx_t)1 << shift) - 1);

        seq_t xs = (seq_t)x_idx;
        seq_t ys = (seq_t)y_idx;

        seq_t xsq = xs * xs;
        seq_t ysq = ys * ys;
        seq_t diff = (xsq > ysq) ? (xsq - ysq) : (ysq - xsq);

        if (first || diff < best) {
            best = diff;
            first = 0;
        }
    }
    return best;
}

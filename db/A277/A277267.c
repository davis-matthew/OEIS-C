/*
 * A277267  Minimum number of single-direction edges in leveled binary trees
 *          with n nodes.
 * https://oeis.org/A277267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(h-1) - 1 + (n - 3*2^(h-1) + 1)*H(n - 3*2^(h-1) + 1)
 *            where h = floor(log₂(n)) and H(x) is the Heaviside step function
 * Range:     u8 n <= 767, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return floor(log₂(v)) for a 64‑bit unsigned integer. */
static inline unsigned floor_log2_idx(idx_t v)
{
    unsigned r = 0;
    while (v >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* a(1) = 0 */

    unsigned h = floor_log2_idx(n);       /* h = ⌊log₂(n)⌋  (h ≥ 1 for n ≥ 2) */
    idx_t pow2 = ((idx_t)1) << (h - 1);   /* 2^(h-1)  */
    idx_t a1 = pow2 - 1;                  /* 2^(h-1) – 1  */
    idx_t thr = (3 * pow2) - 1;           /* 3·2^(h-1) – 1 */
    idx_t diff = 0;
    if (n >= thr) diff = n - thr;         /* n – thr when it is non‑negative */

    return (seq_t)(a1 + diff);
}

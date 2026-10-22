/*
 * A368357  Consider the doubly-infinite permutation P defined on page 87 of
 *          Davis et al. (1977); sequence gives the terms starting at and to
 *          the right of 1.
 * https://oeis.org/A368357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 << e) + reverse_bits(n - cum_prev, e) where e = 0 for
 *            the first block, and for block i ≥ 1, e = 2*i - 1, cum_prev =
 *            Σ_{j=0}^{i-1} 2^{e_j} (e_j as above)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A368357 (right side of the permutation P). */
#include <stdint.h>

static inline uint64_t reverse_bits_uint64(uint64_t x, unsigned width) {
    uint64_t rev = 0;
    while (width--) {
        rev = (rev << 1) | (x & 1ULL);
        x >>= 1;
    }
    return rev;
}

seq_t a(idx_t n) {
    /* Find block index i and size of cumulative elements up to the previous block. */
    idx_t cum = 0;            /* cumulative count of elements before the current block */
    idx_t block = 0;          /* current block index */
    idx_t block_size;         /* size of current block */

    for (;;) {
        block_size = (block == 0) ? (idx_t)1 : ((idx_t)1 << ((idx_t)(2 * block - 1)));
        if (cum + block_size > n)
            break;
        cum += block_size;
        ++block;
    }

    /* Offset within the found block. */
    idx_t offset = n - cum;
    unsigned e = (block == 0) ? 0 : (unsigned)(2 * block - 1);

    /* Reverse the lower‑e bits of the offset. */
    uint64_t rev = reverse_bits_uint64((uint64_t)offset, e);

    /* Base value of the block (start of the block). */
    seq_t base = ((seq_t)1 << e);

    return base + (seq_t)rev;
}

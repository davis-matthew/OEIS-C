/*
 * A368358  Consider the doubly-infinite permutation P defined on page 87 of
 *          Davis et al. (1977); sequence gives the terms starting at and to
 *          the left of 1, in reverse order.
 * https://oeis.org/A368358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A368358:  permutation of the positive integers avoiding 4‑term arithmetic progressions.
    The sequence is obtained by partitioning the indices into blocks of size 4^k (k ≥ 0).
    For a block k (values 4^k … 4^{k+1}−1) the offset within the block is the binary
    reversal of the block index (which uses 2·k bits).  The term at position n is then

       a(n) = 4^k + reverse_bits(n - S(k), 2·k),

    where S(k) = (4^k − 1)/3 is the total number of terms in the previous blocks.  */
/*  Author: ChatGPT (based on the constructive definition in Davis et al.) */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the block k containing index n.
       block = 4^k, sum = total terms in all previous blocks (S(k)) */
    idx_t k = 0;
    idx_t sum = 0;
    idx_t block = 1;          /* 4^0 */
    while (sum + block <= n) {
        sum += block;         /* advance to next block */
        block *= 4;           /* 4^(k+1) */
        ++k;
    }

    /* Offset within the block: binary reversal of t using 2·k bits */
    idx_t t = n - sum;        /* 0 ≤ t < 4^k */
    seq_t offset = 0;
    idx_t bits = 2 * k;
    idx_t tmp = t;
    for (idx_t i = 0; i < bits; ++i) {
        offset = (offset << 1) | (tmp & 1);
        tmp >>= 1;
    }

    /* Base value of the block: 4^k = 2^(2k) */
    seq_t base = (seq_t)block;   /* block holds 2^(2k) in idx_t form */
    return base + offset;
}

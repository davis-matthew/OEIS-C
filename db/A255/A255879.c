/*
 * A255879  Partial sums of A256188.
 * https://oeis.org/A255879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Σ_{k=1..n} b(k), where b(k) is defined by the following
 *            block construction: * Blocks are indexed by k = 1,2,3,... *
 *            Block k starts at index n_k = k² - k + 1 and has length 2k. *
 *            For an element i in block k, let r = i - n_k (0‑based position
 *            in the block). * If r < k → b(i) = r + 1 (value...
 * Range:     u8 n <= 37, u16 n <= 686, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k = 1;                         /* current block index     */
    idx_t block_start = 1;               /* n_k for current block   */
    idx_t next_block = 1 + 2 * k;        /* start of the next block */

    for (idx_t i = 1; i <= n; ++i) {
        /* start a new block when i reaches the start of next block */
        if (i == next_block) {
            ++k;
            block_start = i;
            next_block = block_start + 2 * k;
        }

        idx_t r = i - block_start;      /* position within the current block (0‑based) */

        seq_t val;
        if (r < k) {
            val = (seq_t)(r + 1);
        } else {
            seq_t sk = (seq_t)k * ((seq_t)k + 1) / 2;   /* k(k+1)/2 in seq_t */
            val = sk + (seq_t)(r - k + 1);
        }

        sum += val;
    }

    return sum;
}

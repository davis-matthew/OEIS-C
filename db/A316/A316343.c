/*
 * A316343  Concatenation of S[1], S[2], S[3], ..., where S[k] = 0 followed by
 *          m 1's, where m = (v+1)^2, v = exponent of highest power of 2
 *          dividing k.
 * https://oeis.org/A316343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   block_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 0 if the position n lies at the start of a block S[k];
   otherwise 1; 
   blocks S[k] have length 1+(v+1)^2 where v = exponent of 2 dividing k */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/block_len.h"

/* seq_t a(idx_t n)  -- external linkage */
seq_t a(idx_t n)
{
    idx_t cum = 0;    /* cumulative length through previous blocks */
    idx_t k   = 1;    /* current block index */

    /* Find block containing position n */
    while (cum <= n) {
        idx_t len = block_len(k);
        cum += len;
        ++k;
    }
    /* k is now the first block whose cumulative length > n */
    idx_t len_prev = block_len(k - 1);
    idx_t start_prev = cum - len_prev;
    idx_t offset = n - start_prev;      /* position within block S[k-1] */

    /* In each block, first bit is 0, rest are 1 */
    return (offset == 0) ? 0 : 1;
}

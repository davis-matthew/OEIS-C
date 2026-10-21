/*
 * A244217  Numbers k less than twice the largest Catalan number that is less
 *          than or equal to k.
 * https://oeis.org/A244217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 6917, u32 n <= 6918, u64 n <= 6918,
 *            u128 n <= 6918, big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A244217: a(n) = the n-th integer k such that if m is the largest
   Catalan number <= k then k < 2*m.
   Implementation: precompute the Catalan numbers C_i (i>=2),
   each contributes a block of size C_i, starting at C_i.
   The blocks are [C_0,1], [C_2,2-C_2+1], [C_3,5-?], etc.
   The nth term lies in the block whose cumulative count >= n.
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/catalan.h"

static seq_t *starts = NULL;      /* start of each block (Catalan number)   */
static idx_t *cum    = NULL;      /* cumulative count up to that block      */
static size_t   cap   = 0;         /* allocated capacity of the arrays       */
static size_t   len   = 0;         /* number of blocks currently stored      */

/* expand the precomputed arrays until cumulative >= a_limit */
static void
ensure_precomputed(idx_t a_limit)
{
    if (len == 0) {
        /* initialise first block: C0 = 1 */
        cap = 8;
        starts = (seq_t *)malloc(cap * sizeof(seq_t));
        cum    = (idx_t *)malloc(cap * sizeof(idx_t));
        if (!starts || !cum) abort();        /* memory failure */
        starts[0] = (seq_t)1;
        cum[0]    = (idx_t)1;
        len = 1;
    }

    /* grow until we have enough terms */
    while (cum[len - 1] < a_limit) {
        /* compute next block */
        idx_t   block_start_idx;
        seq_t   block_start;
        idx_t   block_count;
        idx_t   new_cum;

        /* block index = len
           - block 0 already handled
           - block 1 corresponds to Catalan(2)
           - block 2 corresponds to Catalan(3)
           etc. */
        block_start_idx = len + 1;                /* i+1 for i>=1 */
        block_start = catalan(block_start_idx);   /* C_{len+1}   */
        block_count = (idx_t)block_start;         /* size = C_i   */

        new_cum = cum[len - 1] + block_count;

        /* expand storage if needed */
        if (len >= cap) {
            cap = cap * 2;
            starts = (seq_t *)realloc(starts, cap * sizeof(seq_t));
            cum    = (idx_t *)realloc(cum,    cap * sizeof(idx_t));
            if (!starts || !cum) abort();
        }

        starts[len] = block_start;
        cum[len]    = new_cum;
        len++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, but handled */

    ensure_precomputed(n);

    /* linear search – the number of blocks is tiny (≈10) */
    size_t i = 0;
    while (i < len && cum[i] < n) i++;

    idx_t prev_cum = (i == 0) ? 0 : cum[i - 1];
    idx_t offset   = n - prev_cum - 1;          /* 0‑based inside block */

    return starts[i] + (seq_t)offset;
}

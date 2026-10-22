/*
 * A320560  Number of set partitions of [n] such that for each block b the
 *          smallest integer interval containing b has at most ten elements
 *          and for at least one block c the smallest integer interval
 *          containing c has exactly ten elements.
 * https://oeis.org/A320560
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276726(n) – A276725(n)
 * Range:     u8 none, u16 n <= 10, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   count_partitions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A276726(n) = number of set partitions of [n] where every block's
 * smallest‑containing integer interval has length ≤ 10.
 * A276725(n) = same with length ≤ 9.
 * Hence a(n) counts partitions with at least one block of exact
 * interval length 10.  The function works for all n ≥ 10.
 *
 * The implementation uses a state DP over a bitmask of currently
 * "open" blocks.  For a limit k the mask has k‑1 bits, one for each
 * possible distance from the current element to the start of an
 * open block.  At step i (element i+1) each open block can either
 * be closed or extended; a new block may be started.
 * This is a standard Fubini‑like DP with O(n·2^{k−1}) operations.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/count_partitions.h"

seq_t a(idx_t n)
{
    if (n < 10)      /* offset is 10; behaviour for smaller n is irrelevant */
        return 0;

    seq_t full = count_partitions(n, 10);   /* blocks ≤10 */
    seq_t short_len = count_partitions(n, 9); /* blocks ≤9 */
    return full - short_len;
}

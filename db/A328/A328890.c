/*
 * A328890  Number of acyclic edge covers of the complete bipartite graph
 *          K_{n,2}.
 * https://oeis.org/A328890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*2^(n-1) - 2 (offset 1)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"   // provides seq_t pow2(idx_t exp)

/* Return the n-th term of sequence A328890. */
seq_t a(idx_t n)
{
    if (n == 0)          /* not part of the sequence */
        return 0;
    /* Compute (n+2)*2^(n-1) - 2 */
    seq_t pow2_val = pow2(n - 1);          /* 2^(n-1) as seq_t  */
    seq_t factor   = (seq_t)n + (seq_t)2;  /* n+2          */
    seq_t prod     = factor * pow2_val;    /* (n+2)*2^(n-1) */
    prod -= (seq_t)2;                      /* subtract 2    */
    return prod;
}

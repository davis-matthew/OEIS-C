/*
 * A261453  Near-repdigit palindromes with an odd number of digits and all
 *          digits except the middle digit equal.
 * https://oeis.org/A261453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 126, u32 n <= 324, u64 n <= 729,
 *            u128 n <= 1485, big any n
 * Helpers:   build_near_repdigit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261453: near-repdigit palindromes with odd length.  
   For k ≥ 1, d = 1 … 9, m ∈ {0 … 9} \ {d}, the term is obtained by
   concatenating k copies of digit d, digit m, and k copies of d again.
   The sequence lists these numbers in increasing order:
   first all k = 1 terms, then all k = 2 terms, etc. */

#include <stdint.h>
#include "../utils/build_near_repdigit.h"

seq_t a(idx_t n)
{
    /* Each block (fixed k) contains 9 (choices of d) × 9 (choices of m) = 81 terms. */
    idx_t block  = (n - 1) / 81;  /* 0‑based block index, 0 for k = 1 */
    idx_t k      = block + 1;      /* block length (k ≥ 1)               */
    idx_t offset = (n - 1) % 81;   /* position within the block          */

    /* Determine d (1‑9) and m (0‑9, m ≠ d) from the offset. */
    int d = (offset / 9) + 1;                     /* d = 1 … 9                         */
    idx_t r = offset % 9;                        /* 0 … 8                             */
    int m = (r < d) ? (int)r : (int)(r + 1);     /* m skips the value equal to d      */

    return build_near_repdigit(k, d, m);
}

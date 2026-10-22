/*
 * A373948  Run-compression encoded as a transformation of compositions in
 *          standard order.
 * https://oeis.org/A373948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = run‑compression of the n‑th standard composition, encoded
 *            back to an integer
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complexity: O(number of set bits in n) – O(log n).  */

#include <stdint.h>   /* for NULL, uint64_t */

/* idx_t and seq_t are supplied by the build.  */
seq_t a(idx_t n)
{
    /* ---- decode the n-th standard composition ---- */
    /* 1. Find positions of 1‑bits (LSB index starts at 1). */
    idx_t                tmp   = n;    
    idx_t                pos   = 1;    /* current bit position (1‑based) */
    idx_t                prev  = 0;    /* previous position of a 1‑bit */
    idx_t                parts[64];
    int                   len   = 0;

    while (tmp) {
        if (tmp & 1ULL) {
            parts[len++] = pos - prev;   /* the part size */
            prev        = pos;
        }
        tmp >>= 1;
        ++pos;
    }

    /* ---- run‑compress the parts ---- */
    idx_t                comp[64];
    int                   clen  = 0;
    for (int i = 0; i < len; ++i) {
        if (clen == 0 || parts[i] != comp[clen-1]) {
            comp[clen++] = parts[i];
        }
    }

    /* ---- encode the compressed composition back to an integer ---- */
    uint64_t              res   = 0;
    idx_t                 cum   = 0;
    for (int i = 0; i < clen; ++i) {
        cum += comp[i];                     /* cumulative sum of parts   */
        res |= ((uint64_t)1ULL) << (cum - 1);  /* set bit at position cum-1 (0‑based) */
    }

    return (seq_t)res;
}

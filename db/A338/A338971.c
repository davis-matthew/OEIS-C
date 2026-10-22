/*
 * A338971  Linear representation of the complete labeled binary trees of all
 *          integer heights, where the nodes at level i, 0 <= i <= n, of a
 *          binary tree with height n are labeled with the number n-i.
 * https://oeis.org/A338971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the infinite stream obtained by concatenating
   breadth‑first labels of complete binary trees of heights
   0,1,2,… .  For a given index n we find the tree of height h
   that contains the n‑th element, let k be the offset inside
   that tree, and return its label h – ⌊log₂(k+1)⌋.  This
   directly implements the definition. */

#include <stdint.h>

static inline unsigned int floor_log2_u64(uint64_t x)
{
    /* floor(log2(x)), x>0 */
    unsigned int r = 0;
    while (x >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Find the height h such that the cumulative length of trees
       up to height h-1 is <= n < cumulative length up to height h. */
    seq_t cum = 0;          /* cumulative length processed so far   */
    idx_t h = 0;            /* current height being examined       */

    while (1) {
        seq_t tree_size = ((seq_t)1 << (unsigned)(h + 1)) - 1; /* size of tree height h */
        if (cum + tree_size > (seq_t)n) break;                 /* n lies inside this tree */
        cum += tree_size;
        ++h;
    }

    /* Offset within the tree of height h */
    idx_t offset = (idx_t)(n - (idx_t)cum);

    /* Label of node at that offset in breadth‑first order
       is h - floor_log2(offset+1) */
    seq_t label = (seq_t)h - (seq_t)floor_log2_u64((uint64_t)offset + 1);
    return label;
}

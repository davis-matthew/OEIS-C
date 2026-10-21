/*
 * A284656  1-limiting word of the morphism 0 -> 1, 1 -> 0110.
 * https://oeis.org/A284656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A284656
   1-limiting word of the morphism 0 → 1, 1 → 0110.
   The terms are the symbols of the even‑iteration limiting word,
   starting from the 1‑word.  Each term is 0 or 1. */

#include <stddef.h>

/* Child patterns for the two symbols under the composite morphism
   Φ = θ², where θ: 0 → 1, 1 → 0110.  Φ(0) = 0110, Φ(1) = 1011001101. */
static const int child0[4] = {0, 1, 1, 0};
static const int child1[10] = {1, 0, 1, 1, 0, 0, 1, 1, 0, 1};

/* Return the n-th term of the sequence (n ≥ 1).  The index n is
   1‑based as in the OEIS entry. */
seq_t a(idx_t n)
{
    /* Build the lengths of Φ‑expansions of the symbols 0 and 1
       until the length of the expansion of 1 is at least n. */
    static idx_t len0[64];
    static idx_t len1[64];
    int depth = 0;
    len0[0] = len1[0] = 1;           /* after 0 iterations */
    while (len1[depth] < n) {
        int d = depth + 1;
        len0[d] = 2 * len0[depth] + 2 * len1[depth];
        len1[d] = 4 * len0[depth] + 6 * len1[depth];
        depth = d;
    }

    /* Descend the expansion tree from the initial symbol 1. */
    int symbol = 1;     /* 0 or 1 */
    idx_t pos = n;      /* 1‑based position within the current subtree */

    while (depth > 0) {
        if (symbol == 0) {
            /* Φ(0) = 0,1,1,0 (4 children) */
            for (int i = 0; i < 4; ++i) {
                idx_t child_len = (child0[i] == 0) ? len0[depth - 1] : len1[depth - 1];
                if (pos <= child_len) {
                    symbol = child0[i];
                    depth--;
                    break;
                }
                pos -= child_len;
            }
        } else { /* symbol == 1 */
            /* Φ(1) = 1,0,1,1,0,0,1,1,0,1 (10 children) */
            for (int i = 0; i < 10; ++i) {
                idx_t child_len = (child1[i] == 0) ? len0[depth - 1] : len1[depth - 1];
                if (pos <= child_len) {
                    symbol = child1[i];
                    depth--;
                    break;
                }
                pos -= child_len;
            }
        }
    }

    return (seq_t)symbol;   /* 0 or 1 */
}

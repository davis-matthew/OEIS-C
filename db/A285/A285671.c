/*
 * A285671  Fixed point of the morphism 0->11, 1->1110.
 * https://oeis.org/A285671
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

/* Definition: a(n) is the n‑th symbol (0 or 1) of the fixed point of
 * the morphism 0 → 11, 1 → 1110.  The fixed point is obtained by
 * iterating the morphism starting from symbol 1.  a(1) = 1, a(2) = 1, ... */

#include <stdlib.h>

/* seq_t and idx_t are provided by the build system */
seq_t a(idx_t n)
{
    /* The input index is 1‑based; for safety handle n < 1. */
    if (n < 1) return 0;

    /* Pre‑compute lengths of expansions for symbols 0 and 1 at
     * increasing depths until the length of the expansion of 1
     * exceeds or equals n.  len0[d] = |0 after d morphisms|,
     * len1[d] = |1 after d morphisms|. */
    size_t cap = 1;
    idx_t *len0 = (idx_t *)malloc(cap * sizeof(idx_t));
    idx_t *len1 = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!len0 || !len1) return 0;          /* allocation failure */

    len0[0] = 1;            /* a single symbol has length 1 */
    len1[0] = 1;

    size_t d = 0;
    while (len1[d] < n) {           /* grow arrays as needed */
        if (d + 1 == cap) {
            cap *= 2;
            idx_t *tmp0 = (idx_t *)realloc(len0, cap * sizeof(idx_t));
            idx_t *tmp1 = (idx_t *)realloc(len1, cap * sizeof(idx_t));
            if (!tmp0 || !tmp1) {
                free(len0);
                free(len1);
                return 0;
            }
            len0 = tmp0;
            len1 = tmp1;
        }
        /* recurrence for the lengths */
        len0[d + 1] = 2 * len1[d];
        len1[d + 1] = 3 * len1[d] + len0[d];
        ++d;
    }

    size_t depth = d;   /* depth of recursion needed */
    int symbol = 1;     /* start with the root symbol 1 */
    idx_t pos = n;      /* current 1‑based position within the subtree */

    /* Walk down the expansion tree to find the symbol at position n. */
    while (depth > 0) {
        idx_t child_len = (symbol == 0) ? len1[depth - 1] : len1[depth - 1];
        if (symbol == 0) {
            /* 0 expands to [1,1] */
            if (pos > child_len)
                pos -= child_len;     /* second child */
            symbol = 1;
        } else {
            /* 1 expands to [1,1,1,0] */
            if (pos <= child_len) {
                symbol = 1;
            } else if (pos <= 2 * child_len) {
                pos -= child_len;
                symbol = 1;
            } else if (pos <= 3 * child_len) {
                pos -= 2 * child_len;
                symbol = 1;
            } else {
                pos -= 3 * child_len;
                symbol = 0;
            }
        }
        --depth;
    }

    free(len0);
    free(len1);
    return (seq_t)symbol;
}

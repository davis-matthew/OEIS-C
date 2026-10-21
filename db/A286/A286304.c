/*
 * A286304  Number of connected induced (non-null) subgraphs of the complete
 *          binary tree with n nodes.
 * https://oeis.org/A286304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1..n} f(i) where f(i) = \prod_{c \in {2i,2i+1}
 *            \cap [1,n]} (1 + f(c)), with f(i) = 1 for leaves. This counts
 *            the connected induced subgraphs of the binary heap tree
 *            (complete binary tree with nodes 1..n)
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 53, u64 n <= 108,
 *            u128 n <= 216, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for f[i] with indices 1..n.
       Use idx_t for indices – unsigned – so loop terminates correctly.   */
    seq_t *f = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!f) return 0;    /* memory failure – unlikely but safe */

    seq_t ans = 0;

    /* Compute f[i] bottom‑up.  For each node i, the children are 2i
       and 2i+1 if they exist (i.e., <= n).  The term for a leaf is 1. */
    for (idx_t i = n; i; --i) {
        seq_t left  = 1;  /* factor for child 2i */
        seq_t right = 1;  /* factor for child 2i+1 */

        idx_t child = i << 1;              /* 2i */
        if (child <= n) left = (seq_t)1 + f[child];

        child = (i << 1) | 1;              /* 2i+1 */
        if (child <= n) right = (seq_t)1 + f[child];

        f[i] = left * right;
        ans  += f[i];
    }

    free(f);
    return ans;
}

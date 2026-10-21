/*
 * A273736  Number of permutations of n elements divided by the number of
 *          8-ary heaps on n+1 elements.
 * https://oeis.org/A273736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000142(n)/A273695(n+1)
 * Range:     u8 n <= 26, u16 n <= 48, u32 n <= 83, u64 n <= 155,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the product of all subtree sizes of the 8‑ary heap
   on (n+1) nodes, excluding the root. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)-1) return 0;           /* for robustness on overflow */
    idx_t N = n + 1;                         /* number of nodes in the heap */

    /* Allocate an array to store subtree sizes. */
    idx_t *sz = (idx_t*)malloc(sizeof(idx_t) * N);
    if (!sz) return 0;                       /* allocation failure */

    seq_t prod = (seq_t)1;                    /* product of non‑root sizes */

    /* Compute sizes bottom‑up: from last index down to 0. */
    for (idx_t i = N; i-- > 0;) {
        seq_t s = 1;                          /* include this node */
        idx_t base = 8 * i;                   /* children indices start at 8*i+1 */

        /* Add sizes of existing children. */
        for (idx_t j = 1; j <= 8; ++j) {
            idx_t child = base + j;
            if (child >= N) break;            /* child index out of bounds */
            s += sz[child];
        }

        sz[i] = (idx_t)s;                    /* store size for possible parent use */

        if (i != 0)                          /* exclude root from product */
            prod *= sz[i];
    }

    free(sz);
    return prod;
}

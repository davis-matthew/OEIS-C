/*
 * A399661  a(n) is the number of ordered forests of rooted trees on n nodes
 *          where one node in the first tree is marked.
 * https://oeis.org/A399661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = Sum_{j=1..n} j * A000081(j) * A000107(n-j)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000081: number of rooted trees with j nodes              */
/* A000107: number of unordered forests of rooted trees      */

/* External utilities */
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* -------------  Cached arrays  ----------------- */
static seq_t *root = NULL;       /* A000081 up to current capacity   */
static seq_t *fa   = NULL;       /* A000107 up to current capacity   */
static idx_t  cap  = 0;          /* current capacity (max n)         */

/* Re‑allocate arrays when we need a larger n */
static void need_capacity(idx_t n)
{
    if (n <= cap) return;                     /* already enough */

    /* free old arrays */
    if (root) free_seq_array(root);
    if (fa)   free_seq_array(fa);

    /* allocate new ones */
    root = alloc_seq_array(n + 1);
    fa   = alloc_seq_array(n + 1);

    /* recompute all values up to n */
    /* ---- A000081 (rooted trees) ---- */
    /* divS[k] = Σ_{d|k} d * root[d] */
    seq_t *divS = alloc_seq_array(n + 1);
    for (idx_t i = 0; i <= n; ++i) {
        root[i] = 0;
        divS[i] = 0;
    }
    root[1] = 1;
    /* init divisor sums for multiples of 1 */
    seq_t add = 1 * root[1];
    for (idx_t k = 1; k <= n; ++k)
        divS[k] += add;

    for (idx_t m = 2; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k < m; ++k)
            sum += divS[k] * root[m - k];
        root[m] = sum / (seq_t)(m - 1);

        seq_t m_seq = (seq_t)m;
        add = m_seq * root[m];
        for (idx_t k = m; k <= n; k += m)
            divS[k] += add;
    }
    free_seq_array(divS);

    /* ---- A000107 (unordered forests) ---- */
    fa[0] = 1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k)
            sum += root[k] * fa[m - k];
        fa[m] = sum;
    }

    cap = n;           /* new capacity */
}

/* Main function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    need_capacity(n);          /* ensure arrays hold 0..n */

    seq_t res = 0;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t j_seq = (seq_t)j;
        res += j_seq * root[j] * fa[n - j];
    }
    return res;
}

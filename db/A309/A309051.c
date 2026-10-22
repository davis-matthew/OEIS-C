/*
 * A309051  Total number of 0's in all (binary) max-heaps on n elements from
 *          the set {0,1}.
 * https://oeis.org/A309051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 46, u64 n <= 99, u128 n <= 206,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: For a max‑heap of n nodes, let
   f[u] = number of binary max‑heap assignments in the subtree
          rooted at node u,
   g[u] = total number of zeros over all such assignments.
   For a node u with children c:
       f[u] = 1 + Π_{c} f[c]            (1 for the case u=0,
                                         otherwise product of children's f)
       g[u] = size(u) + Σ_{c} g[c] · Π_{i≠c} f[i]
   Leaf: f = 2, g = 1.
   The answer a(n) is g[0].
*/
#include <stdlib.h>

typedef struct {
    seq_t f;
    seq_t g;
    idx_t sz;   /* number of nodes in the subtree */
} heap_info_t;

static heap_info_t dfs_info(idx_t u, idx_t n)
{
    idx_t left  = 2*u + 1;
    idx_t right = 2*u + 2;
    heap_info_t li = {1, 0, 0}, ri = {1, 0, 0};
    int hasL = 0, hasR = 0;

    if (left  < n) { li = dfs_info(left,  n); hasL = 1; }
    if (right < n) { ri = dfs_info(right, n); hasR = 1; }

    seq_t prod_f = 1;
    if (hasL) prod_f *= li.f;
    if (hasR) prod_f *= ri.f;

    seq_t f_u = prod_f + 1;
    idx_t sz_u = 1;
    if (hasL) sz_u += li.sz;
    if (hasR) sz_u += ri.sz;

    seq_t sum = 0;
    if (hasL) sum += li.g * (prod_f / li.f);   /* prod_f/li.f == ri.f or 1 */
    if (hasR) sum += ri.g * (prod_f / ri.f);

    seq_t g_u = (seq_t)sz_u + sum;

    heap_info_t res = {f_u, g_u, sz_u};
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    heap_info_t root = dfs_info(0, n);
    return root.g;
}

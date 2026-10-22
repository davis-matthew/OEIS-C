/*
 * A340010  The order of the largest weakly connected component of the Collatz
 *          digraph of order n.
 * https://oeis.org/A340010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 497, u16 n <= 7399, u32 n <= 7399, u64 n <= 7399,
 *            u128 n <= 7399, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the order of the largest weakly connected component
   of the Collatz digraph of order n   (OEIS A340010). */
#include <stdlib.h>

/* Find root in a disjoint‑set forest with path halving. */
static idx_t
find_root(idx_t x, idx_t *par)
{
    while (par[x] != x) {
        par[x] = par[par[x]];   /* path halving */
        x = par[x];
    }
    return x;
}

/* Sequence value a(n) */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* offset is 1; below is undefined */

    /* Allocate DSU arrays */
    idx_t *par = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!par)
        return (seq_t)0;
    idx_t *sz  = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!sz) {
        free(par);
        return (seq_t)0;
    }

    /* Initialise each vertex as a singleton component */
    for (idx_t i = 1; i <= n; ++i) {
        par[i] = i;
        sz[i]  = 1;
    }

    /* For every m in 1..n, connect it with its Collatz successor */
    for (idx_t m = 1; m <= n; ++m) {
        idx_t succ = (m & 1ULL) ? ((3 * m + 1) >> 1) : (m >> 1);
        if (succ <= n) {
            idx_t r1 = find_root(m,   par);
            idx_t r2 = find_root(succ,par);
            if (r1 != r2) {
                /* Union by size */
                if (sz[r1] < sz[r2]) {
                    idx_t tmp = r1; r1 = r2; r2 = tmp;
                }
                par[r2] = r1;
                sz[r1] += sz[r2];
            }
        }
    }

    /* Find the largest component size */
    idx_t max_sz = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (par[i] == i && sz[i] > max_sz)
            max_sz = sz[i];
    }

    free(par);
    free(sz);
    return (seq_t)max_sz;
}

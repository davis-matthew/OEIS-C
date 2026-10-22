/*
 * A398602  Maximum size of a subset of {1, 2, ..., n} containing n, such that
 *          the sum of any two distinct elements is a perfect power.
 * https://oeis.org/A398602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is computed by a search for the largest subset of {1,…,n}
 *            containing n such that every pair sums to a perfect power. The
 *            algorithm enumerates all candidates x < n with n+x a perfect
 *            power, then finds a maximum clique in the graph of candidates
 *            where an edge exists iff the sum of the two cand...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*------------------------------------------------------------------*/
/* Recursive search for maximum clique among 'cand' vertices.        */
/*------------------------------------------------------------------*/
static void dfs(uint64_t mask, int curSize, int numCand,
                const uint64_t *adj, int *bestClique)
{
    if (mask == 0) {
        if (curSize > *bestClique) *bestClique = curSize;
        return;
    }

    /* upper bound pruning */
    int remaining = __builtin_popcountll(mask);
    if (curSize + remaining <= *bestClique) return;

    /* choose first vertex in mask */
    uint64_t vMask = mask & -mask;           /* lowest set bit */
    int      v     = __builtin_ctzll(mask);  /* its index */

    /* include v */
    dfs(mask & adj[v], curSize + 1, numCand, adj, bestClique);
    /* exclude v */
    dfs(mask & ~vMask, curSize, numCand, adj, bestClique);
}

seq_t a(idx_t n)
{
    /* limit for perfect powers: sums are at most 2*n-1 */
    idx_t L = 2 * n;
    if (L < 2) L = 2;

    /* isPower[s] = true iff s is a perfect power p^k with k>=2   */
    bool *isPower = (bool*)calloc((size_t)(L + 1), sizeof(bool));
    if (!isPower) return 0;   /* allocation failure, should not happen */

    for (idx_t base = 2; base * base <= L; ++base) {
        idx_t p = base * base;
        while (p <= L) {
            isPower[p] = true;
            if (p > L / base) break;  /* avoid overflow of next power */
            p *= base;
        }
    }

    /* gather candidate numbers x < n such that n + x is a perfect power */
    idx_t cand[64];
    int    numCand = 0;
    for (idx_t s = 2; s <= L; ++s) {
        if (!isPower[s]) continue;
        if (s <= n) continue;          /* would give x <= 0 */
        idx_t x = s - n;
        if (x >= n) continue;          /* must be < n */
        /* avoid duplicates */
        bool dup = false;
        for (int i = 0; i < numCand; ++i)
            if (cand[i] == x) { dup = true; break; }
        if (!dup && numCand < 64) cand[numCand++] = x;   /* safety limit */
    }

    /* adjacency masks for candidates (only up to 64 vertices) */
    uint64_t adj[64] = {0};
    for (int i = 0; i < numCand; ++i)
        for (int j = i + 1; j < numCand; ++j) {
            idx_t sum = cand[i] + cand[j];
            if (sum <= L && isPower[sum]) {
                adj[i] |= 1ULL << j;
                adj[j] |= 1ULL << i;
            }
        }

    int bestClique = 0;
    if (numCand > 0) {
        uint64_t initMask = (numCand == 64) ? ~0ULL : ((1ULL << numCand) - 1);
        dfs(initMask, 0, numCand, adj, &bestClique);
    }

    free(isPower);
    return (seq_t)(bestClique + 1);   /* include n itself */
}

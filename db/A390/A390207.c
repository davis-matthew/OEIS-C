/*
 * A390207  Let T_n be a rooted tree constructed in the following way. With
 *          k_0 = n as the root, let k_i be a vertex of depth i. For each
 *          prime divisor p of k_i, the children of k_i are those m = k_i +
 *          (-1)^i*k_i/p, where m >= 1 and m does not appear in the path from
 *          k_0 to k_i. a(n) is the number of paths i...
 * https://oeis.org/A390207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of root-to‑leaf paths in the tree defined by OEIS
 *            A390207. The tree is finite for every n ≥ 1. The implementation
 *            performs a depth‑first search exploring each possible path,
 *            using the ancestor set to avoid revisiting previously seen
 *            nodes. This method works for all n that fit in an...
 * Range:     u8 n <= 109, u16 n <= 1033, u32 n <= 3101, u64 n <= 3101,
 *            u128 n <= 3101, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static seq_t dfs(uint64_t cur, int parity, uint64_t *anc, size_t depth)
{
    /* Get distinct prime divisors of cur. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt;
    factor_u64(cur, primes, exps, &pf_cnt);

    seq_t total = 0;
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p     = primes[i];
        uint64_t factor = cur / p;
        uint64_t child = (parity == 0) ? cur + factor : cur - factor;
        if (child == 0) continue;          /* safety: child must be ≥1 */

        /* check that child is not an ancestor on the current path */
        bool visited = false;
        for (size_t j = 0; j < depth; ++j) {
            if (anc[j] == child) {
                visited = true;
                break;
            }
        }
        if (visited) continue;

        anc[depth] = child;
        total += dfs(child, parity ^ 1, anc, depth + 1);
    }

    /* if no children are possible, we have reached a leaf path */
    return (total == 0) ? 1 : total;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, never queried */

    /* Allocate a buffer large enough for the maximum possible depth. */
    const size_t MAX_DEPTH = 4096;
    uint64_t *anc = (uint64_t *)malloc(MAX_DEPTH * sizeof(uint64_t));
    if (!anc) abort();                   /* out of memory */

    anc[0] = (uint64_t)n;
    seq_t result = dfs((uint64_t)n, 0, anc, 1);
    free(anc);
    return result;
}

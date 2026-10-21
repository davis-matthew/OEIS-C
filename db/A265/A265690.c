/*
 * A265690  Cardinality of shortest addition chain for n.
 * https://oeis.org/A265690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003313(n) + 1 where A003313(n) is the length of a
 *            shortest addition chain for n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   dfs_depth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the minimal addition‑chain length (number of steps) of n
   using iterative deepening DFS, caching results for future calls. */
#include <stdlib.h>
#include <string.h>
#include "../utils/dfs_depth.h"

/* static cache for already computed chain lengths (steps) */
static seq_t *len_cache = NULL;
static idx_t len_cache_cap = 0;

/* ensure the cache can hold indices up to n */
static void ensure_len_cache(idx_t n)
{
    if (n < len_cache_cap)
        return;
    idx_t new_cap = len_cache_cap ? len_cache_cap : 1;
    while (new_cap <= n)
        new_cap <<= 1;
    len_cache = (seq_t *)realloc(len_cache, (new_cap + 1) * sizeof(seq_t));
    memset(len_cache + len_cache_cap, 0, (new_cap + 1 - len_cache_cap) * sizeof(seq_t));
    len_cache_cap = new_cap;
    /* empty slots are zero; length for n=1 is zero steps by definition */
}

/* compute the minimal addition‑chain length (steps) for a single n */
static seq_t minimal_chain_len(idx_t n)
{
    if (n == 1)
        return 0;   /* by definition, length of chain 1 is 0 steps */

    ensure_len_cache(n);
    if (len_cache[n] != 0)
        return len_cache[n];   /* cached result */

    /* lower bound on the depth (number of steps) is ceil(log2(n)) */
    idx_t t = 1;
    int lower = 0;
    while (t < n) {
        t <<= 1;
        ++lower;
    }

    idx_t chain[128];
    chain[0] = 1;
    seq_t best = (seq_t)(-1);   /* will be replaced when found */

    for (int depth = lower; depth <= 100; ++depth) {   /* 100 is safely above any needed depth */
        if (dfs_depth(n, depth, 0, chain)) {
            best = (seq_t)depth;
            break;
        }
    }

    len_cache[n] = best;          /* cache for future use */
    return best;
}

/* public function required by the OJ */
seq_t a(idx_t n)
{
    seq_t steps = minimal_chain_len(n);   /* number of steps in shortest chain */
    return steps + (seq_t)1;              /* cardinality = steps + 1 */
}

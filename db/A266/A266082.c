/*
 * A266082  Length of shortest Fibonacci addition chain for n.
 * https://oeis.org/A266082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 233, u16 n <= 233, u32 n <= 233, u64 n <= 233,
 *            u128 n <= 233, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Compute the length of the shortest Fibonacci addition chain
   for n by iterative deepening depth‑first search (no doubling allowed). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cache for memoisation -------------------------------------------- */
static seq_t *cache = NULL;      /* cache[n] stores result for n   */
static bool *computed = NULL;    /* true if cache[n] is valid     */
static size_t cache_cap = 0;     /* allocated capacity (>= n+1)   */

static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    size_t newcap = n + 1;
    /* realloc returns void*, cast for C++ compatibility */
    seq_t *newc   = (seq_t*)realloc(cache,   newcap * sizeof(seq_t));
    bool *newcomp = (bool*)realloc(computed, newcap * sizeof(bool));
    if (!newc || !newcomp) {
        exit(1);   /* allocation failed */
    }

    /* mark new entries as not computed */
    for (size_t i = cache_cap; i < newcap; ++i)
        newcomp[i] = false;

    cache    = newc;
    computed = newcomp;
    cache_cap = newcap;
}

/* Sort array of idx_t in descending order -------------------------------- */
static void sort_desc(idx_t *arr, size_t len)
{
    for (size_t i = 1; i < len; ++i) {
        idx_t key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] < key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

/* Depth‑first search ---------------------------------------------------- */
static bool dfs(idx_t *chain, size_t len, size_t depth, idx_t target)
{
    if (len - 2 == depth)          /* exact length reached */
        return chain[len - 1] == target;

    /* Generate admissible sums (strictly increasing). */
    idx_t cand[256];
    size_t cc = 0;
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i + 1; j < len; ++j) {
            idx_t s = chain[i] + chain[j];
            if (s <= chain[len - 1] || s > target)
                continue;
            /* avoid duplicates */
            bool dup = false;
            for (size_t k = 0; k < cc; ++k) {
                if (cand[k] == s) { dup = true; break; }
            }
            if (!dup) cand[cc++] = s;
        }
    }

    sort_desc(cand, cc);
    for (size_t idx = 0; idx < cc; ++idx) {
        chain[len] = cand[idx];
        if (dfs(chain, len + 1, depth, target))
            return true;
    }
    return false;
}

/* Main function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* safe but unused */
        return 0;

    ensure_cache(n);
    if (computed[n])
        return cache[n];

    /* Base cases */
    if (n == 1) {
        computed[1] = true;
        cache[1] = 0;
        return 0;
    }
    if (n == 2) {
        computed[2] = true;
        cache[2] = 1;
        return 1;
    }

    idx_t target = n;

    /* Lower bound on depth: smallest d with Fibonacci F_{d+2} >= n. */
    idx_t a1 = 1, b1 = 1;   /* F1, F2 */
    size_t d = 0;
    while (b1 < target) {
        idx_t tmp = a1 + b1;
        a1 = b1;
        b1 = tmp;
        ++d;
    }
    size_t depth = d;

    /* Iterative deepening */
    while (1) {
        idx_t chain[32];
        chain[0] = 1;
        chain[1] = 1;
        if (dfs(chain, 2, depth, target)) {
            cache[n] = (seq_t)depth;
            computed[n] = true;
            return (seq_t)depth;
        }
        ++depth;
    }
}

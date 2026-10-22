/*
 * A335521  Number of (1,2,3)-avoiding permutations of the prime indices of n.
 * https://oeis.org/A335521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (1,2,3)-avoiding permutations of the multiset
 *            of prime indices of n. This implementation factorizes n,
 *            collects the multiplicities of each prime index, and counts all
 *            permutations that avoid an increasing subsequence of length 3 by
 *            a depth‑first search that simulates the greedy p...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Utility headers.  Adjust paths if necessary. */
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

/* ----- Hash table for memoization ----- */
typedef struct { uint64_t key; seq_t val; } CacheEntry;

static CacheEntry *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_cnt = 0;

static void cache_reset(size_t min_cap)
{
    /* pick a power of two large enough */
    size_t cap = 1;
    while (cap < min_cap) cap <<= 1;
    if (cache == NULL || cache_cap < cap) {
        free(cache);
        cache = (CacheEntry *)calloc(cap, sizeof(CacheEntry));
        cache_cap = cap;
    } else {
        memset(cache, 0, cache_cap * sizeof(CacheEntry));
    }
    cache_cnt = 0;
}

static bool cache_find(uint64_t key, seq_t *out)
{
    size_t p = key & (cache_cap - 1);
    while (cache[p].key) {
        if (cache[p].key == key) { *out = cache[p].val; return true; }
        p = (p + 1) & (cache_cap - 1);
    }
    return false;
}

static void cache_insert(uint64_t key, seq_t val)
{
    /* simple resize guard – should never hit in practice */
    if ((cache_cnt + 1) * 10 >= cache_cap * 7) {
        size_t new_cap = cache_cap << 1;
        CacheEntry *new_tbl = (CacheEntry *)calloc(new_cap, sizeof(CacheEntry));
        for (size_t i = 0; i < cache_cap; ++i) {
            if (cache[i].key) {
                uint64_t k = cache[i].key;
                size_t p = k & (new_cap - 1);
                while (new_tbl[p].key) p = (p + 1) & (new_cap - 1);
                new_tbl[p].key = k;
                new_tbl[p].val = cache[i].val;
            }
        }
        free(cache);
        cache = new_tbl;
        cache_cap = new_cap;
    }
    size_t p = key & (cache_cap - 1);
    while (cache[p].key) p = (p + 1) & (cache_cap - 1);
    cache[p].key = key;
    cache[p].val = val;
    ++cache_cnt;
}

/* ----- Global variables used by the DFS ----- */
static int g_val[20];     /* distinct prime indices */
static int g_d;           /* number of distinct indices */
static int g_cnt[20];     /* remaining multiplicities */

/* DFS counting function */
static seq_t dfs(int t1, int t2)
{
    /* encode state */
    uint64_t key = 0;
    for (int i = 0; i < g_d; ++i) key = (key << 4) | (uint64_t)g_cnt[i];
    key = (key << 4) | (uint64_t)t1;
    key = (key << 4) | (uint64_t)t2;

    seq_t cached;
    if (cache_find(key, &cached)) return cached;

    /* base case */
    int sum = 0;
    for (int i = 0; i < g_d; ++i) sum += g_cnt[i];
    if (sum == 0) return 1;

    seq_t total = 0;
    for (int i = 0; i < g_d; ++i) {
        if (g_cnt[i] == 0) continue;
        int val = g_val[i];
        int nt1 = t1, nt2 = t2;

        if (t1 == g_d || val <= g_val[t1])
            nt1 = i;                           /* place on pile 1 */
        else if (t2 == g_d || val <= g_val[t2])
            nt2 = i;                           /* place on pile 2 */
        else
            continue;                          /* would need a third pile */

        --g_cnt[i];
        total += dfs(nt1, nt2);
        ++g_cnt[i];
    }

    cache_insert(key, total);
    return total;
}

/* ----- Public sequence function ----- */
seq_t a(idx_t n)
{
    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;

    if (n > 1)
        factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    /* gather multiplicities of prime indices */
    int vals[20];
    int cnts[20];
    int d = 0;

    for (size_t j = 0; j < pf_cnt; ++j) {
        uint64_t p = primes[j];
        uint64_t exp = exps[j];
        int idx = (int)prime_pi(p);   /* primes are prime, so prime_pi(p) is its index */
        int k = -1;
        for (int t = 0; t < d; ++t)
            if (vals[t] == idx) { k = t; break; }
        if (k == -1) {
            vals[d] = idx;
            cnts[d] = (int)exp;
            ++d;
        } else {
            cnts[k] += (int)exp;
        }
    }

    /* special case n == 1 */
    if (n == 1) return 1;

    /* fill global arrays */
    g_d = d;
    for (int i = 0; i < d; ++i) {
        g_val[i] = vals[i];
        g_cnt[i] = cnts[i];
    }

    /* initialize memoization table (capacity arbitrary large enough) */
    cache_reset(1 << 16);

    /* initial piles are empty, use index d as sentinel */
    return dfs(d, d);
}

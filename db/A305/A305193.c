/*
 * A305193  Number of connected factorizations of n.
 * https://oeis.org/A305193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of connected factorizations of n processed by
 *            generating all multiplicative partitions of n and testing
 *            connectivity of the graph G(S) built from prime divisibility of
 *            the factors
 * Range:     u8 n <= 5183, u16 n <= 20736, u32 n <= 20736, u64 n <= 20736,
 *            u128 n <= 20736, big any n
 * Validated: standard, pass, 121 terms through n = 9558 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* DSU find with path compression */
static size_t find_root(size_t *p, size_t i)
{
    while (p[i] != i) {
        p[i] = p[p[i]];
        i = p[i];
    }
    return i;
}

/* test whether the factor multiset is connected */
static bool is_connected(const uint64_t *f,
                         size_t len,
                         const uint64_t *primes,
                         size_t pcnt)
{
    if (len == 0) return false;        /* empty factor set never occurs */
    size_t parent[32];
    uint32_t mask[32];

    for (size_t i = 0; i < len; ++i) {
        parent[i] = i;
        uint32_t m = 0;
        for (size_t j = 0; j < pcnt; ++j) {
            if (f[i] % primes[j] == 0) m |= (1u << j);
        }
        mask[i] = m;
    }

    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i + 1; j < len; ++j) {
            if (mask[i] & mask[j]) {
                size_t ri = find_root(parent, i);
                size_t rj = find_root(parent, j);
                if (ri != rj) parent[rj] = ri;
            }
        }
    }

    size_t root0 = find_root(parent, 0);
    for (size_t i = 1; i < len; ++i)
        if (find_root(parent, i) != root0) return false;
    return true;
}

/* cache of already computed results */
static seq_t *cache = NULL;
static unsigned char *cached = NULL;
static idx_t cache_cap = 0;

/* make sure the cache arrays are large enough */
static void ensure_cache_capacity(idx_t n)
{
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        cache  = mem_resize(seq_t, cache, new_cap);
        cached = (unsigned char *)realloc(cached, new_cap * sizeof(unsigned char));
        for (idx_t i = cache_cap; i < new_cap; ++i) cached[i] = 0;
        cache_cap = new_cap;
    }
}

/* recursive generation of multiplicative partitions of rem
   with min factor at least minf.  cur holds the current factors. */
static void dfs(uint64_t rem,
                uint64_t minf,
                uint64_t *cur,
                size_t cur_len,
                const uint64_t *primes,
                size_t pcnt,
                seq_t *cnt)
{
    for (uint64_t f = minf; f <= rem / f; ++f) {
        if (rem % f == 0) {
            cur[cur_len] = f;
            dfs(rem / f, f, cur, cur_len + 1, primes, pcnt, cnt);
        }
    }

    if (rem >= minf) {                     /* final factor is rem itself   */
        cur[cur_len] = rem;
        if (is_connected(cur, cur_len + 1, primes, pcnt)) ++(*cnt);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* defined only for n>=1          */

    /* item 1 has special value 0 */
    if (n == 1) return 0;

    ensure_cache_capacity(n);
    if (cached[n]) return cache[n];

    /* factorise n to obtain its distinct prime divisors */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* generate all connected factorizations */
    uint64_t cur[32];
    seq_t count = 0;
    dfs((uint64_t)n, 2, cur, 0, primes, pcnt, &count);

    cache[n]  = count;
    cached[n] = 1;
    return count;
}

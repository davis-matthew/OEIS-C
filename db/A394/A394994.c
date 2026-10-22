/*
 * A394994  Matchable numbers: an integer k is matchable if there is a coprime
 *          matching between [1..tau(k)] and D(k), where tau(k) is A000005(k)
 *          and D(k) is the set of divisors of k.
 * https://oeis.org/A394994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A394994: matchable numbers. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_gcd64.h"

/* Euclidean gcd for 64‑bit integers. */

/* Depth‑first search for augmenting paths (Kuhn's algorithm). */
static bool _dfs(int u, size_t t, unsigned char *g,
                 int *matchR, unsigned char *visited)
{
    for (size_t v = 0; v < t; ++v) {
        if (g[u * t + v] && !visited[v]) {
            visited[v] = 1;
            if (matchR[v] == -1 || _dfs(matchR[v], t, g, matchR, visited)) {
                matchR[v] = u;
                return true;
            }
        }
    }
    return false;
}

/* Checks whether k is matchable. */
static bool _is_matchable(uint64_t k)
{
    if (k == 0)
        return false;
    /* Quick necessary condition from the OEIS comments. */
    if (k % 4 == 0 && k > 4)
        return false;

    /* Factorization of k. Small primes suffice for the range needed. */
    uint64_t fac[16];
    int exp[16];
    int pc = 0;                                   /* number of prime factors */
    uint64_t tmp = k;
    if (tmp % 2 == 0) {
        int e = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            ++e;
        }
        fac[pc] = 2;
        exp[pc++] = e;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            fac[pc] = p;
            exp[pc++] = e;
        }
    }
    if (tmp > 1) {                  /* remaining prime */
        fac[pc] = tmp;
        exp[pc++] = 1;
    }

    /* Compute tau(k) and the number of odd divisors. */
    uint64_t tau = 1;
    uint64_t tau_odd = 1;
    for (int i = 0; i < pc; ++i) {
        tau *= (uint64_t)(exp[i] + 1);
        if (fac[i] != 2)
            tau_odd *= (uint64_t)(exp[i] + 1);
    }
    uint64_t even_div = tau - tau_odd;
    uint64_t odd_indices = (tau + 1) / 2;      /* indices 1..tau, odd ones */
    if (even_div > odd_indices)
        return false;                  /* Hall condition fails */

    size_t t = (size_t)tau;

    /* Generate all divisors. */
    uint64_t *div = (uint64_t *)malloc(t * sizeof(uint64_t));
    if (!div)
        return false;
    size_t len = 1;
    div[0] = 1;
    for (int i = 0; i < pc; ++i) {
        uint64_t p = fac[i];
        int e = exp[i];
        size_t prev_len = len;
        uint64_t p_pow = 1;
        for (int f = 1; f <= e; ++f) {
            p_pow *= p;
            for (size_t j = 0; j < prev_len; ++j) {
                div[len++] = div[j] * p_pow;
            }
        }
    }

    /* Build bipartite adjacency: left side indices 1..tau, right side divisors. */
    unsigned char *g = (unsigned char *)malloc(t * t);
    if (!g) {
        free(div);
        return false;
    }
    for (size_t i = 0; i < t; ++i) {
        uint64_t idx = i + 1;
        for (size_t j = 0; j < t; ++j) {
            g[i * t + j] = (_gcd64(idx, div[j]) == 1) ? 1U : 0U;
        }
    }

    /* Kuhn maximal matching. */
    int *matchR = (int *)malloc(t * sizeof(int));
    if (!matchR) {
        free(g);
        free(div);
        return false;
    }
    for (size_t j = 0; j < t; ++j)
        matchR[j] = -1;

    unsigned char *visited = (unsigned char *)malloc(t);
    if (!visited) {
        free(matchR);
        free(g);
        free(div);
        return false;
    }

    bool ok = true;
    for (size_t u = 0; u < t; ++u) {
        for (size_t v = 0; v < t; ++v)
            visited[v] = 0;
        if (!_dfs((int)u, t, g, matchR, visited)) {
            ok = false;
            break;
        }
    }

    free(visited);
    free(matchR);
    free(g);
    free(div);
    return ok;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    static uint64_t next_candidate = 1;
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate++;
        if (_is_matchable(cand)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               cache_cap * sizeof(seq_t));
                if (!tmp)
                    return 0;   /* allocation failure – unlikely in tests */
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }
    return cache[n - 1];
}

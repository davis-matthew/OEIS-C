/*
 * A308260  a(n) = Product_{k=1..n, gcd(n,k) = 1} prime(k).
 * https://oeis.org/A308260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1 ≤ k ≤ n, gcd(n,k)=1} prime(k)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 16, u128 n <= 23,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static idx_t estimate_limit(idx_t n)
{
    if (n < 6) return 15;
    double dn = (double)n;
    double lim = dn * (log(dn) + log(log(dn))) + 10.0;
    return (idx_t)lim;
}

/* Globals to cache primes and computed a(n) */
static uint64_t *g_primes = NULL;   /* array of primes, 1-indexed */
static size_t     g_primes_cnt = 0;
static idx_t      g_primes_limit = 0;

static seq_t *g_cache = NULL;   /* a(n) cache, 0-indexed */
static size_t  g_cache_cap = 0;

static void ensure_primes(idx_t k)
{
    if (k <= g_primes_cnt) return;
    /* Need at least k primes */
    idx_t limit = estimate_limit(k);
    if (limit <= g_primes_limit) limit = g_primes_limit * 2;
    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);
    free(g_primes);
    g_primes = new_primes;
    g_primes_cnt = new_cnt;
    g_primes_limit = limit;
}

static seq_t compute_value(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_primes(n);                     /* primes up to n */
    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd_idx(n, k) == 1) {
            prod *= (seq_t)g_primes[k - 1];
        }
    }
    return prod;
}

seq_t a(idx_t n)
{
    if (n >= g_cache_cap) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        g_cache = new_cache;
        for (size_t i = g_cache_cap; i <= (size_t)n; ++i) {
            g_cache[i] = compute_value((idx_t)i);
        }
        g_cache_cap = new_cap;
    }
    return g_cache[n];
}

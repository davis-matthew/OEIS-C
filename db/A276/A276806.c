/*
 * A276806  Height of the shortest binary factorization tree of n.
 * https://oeis.org/A276806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 1 or prime; otherwise a(n) = 1 + min_{d|n,
 *            1<d<n} ( max(a(d), a(n/d)) ). This recursive definition yields
 *            the height of the shortest binary factorization tree of n.
 *            Property: a(n^2) = a(n) + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Internal helper: prime test using trial division. */
static bool is_prime(idx_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t i = 3; i <= x / i; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

/* Cache for computed values.  The cache is expanded lazily. */
static seq_t *cache = NULL;
static unsigned char *computed = NULL;
static idx_t cache_cap = 0;

static void ensure_cache(idx_t n) {
    if (n + 1 <= cache_cap) return;
    idx_t newcap = n + 1;
#if defined(__cplusplus)
    seq_t *newcache = new seq_t[newcap];
    unsigned char *newcomp = new unsigned char[newcap];
    for (idx_t i = 0; i < newcap; ++i) {
        newcomp[i] = 0;
    }
    if (cache) delete[] cache;
    if (computed) delete[] computed;
    cache = newcache;
    computed = newcomp;
#else
    seq_t *newcache = (seq_t *)malloc((size_t)newcap * sizeof(seq_t));
    unsigned char *newcomp = (unsigned char *)malloc((size_t)newcap * sizeof(unsigned char));
    if (!newcache || !newcomp) exit(1);
    for (idx_t i = 0; i < newcap; ++i) {
        newcomp[i] = 0;
    }
    if (cache) free(cache);
    if (computed) free(computed);
    cache = newcache;
    computed = newcomp;
#endif
    cache_cap = newcap;
}

/* Recursive computation with memoisation. */
static seq_t compute_a(idx_t n) {
    ensure_cache(n);
    if (computed[n]) return cache[n];

    seq_t value = 0;
    if (n <= 1) {
        value = 0;
    } else if (is_prime(n)) {
        value = 0;
    } else {
        bool first = true;
        seq_t best = 0;
        for (idx_t d = 2; d <= n / d; ++d) {
            if (n % d == 0) {
                idx_t e = n / d;
                seq_t h1 = compute_a(d);
                seq_t h2 = compute_a(e);
                seq_t h = (h1 > h2 ? h1 : h2) + 1;
                if (first || h < best) {
                    best = h;
                    first = false;
                }
            }
        }
        value = best;
    }

    cache[n] = value;
    computed[n] = 1;
    return value;
}

/* Public function required by the exercise. */
seq_t a(idx_t n) {
    return compute_a(n);
}

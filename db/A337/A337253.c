/*
 * A337253  a(2n) = b(a(n)^2), a(2n+1) = b(a(n) + a(n - 1)) where b(k) =
 *          abs(k)*(-1)^k; a(0) = 0, a(1) = 1.
 * https://oeis.org/A337253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 0, a(1) = 1; * For n > 0: * a(2n) = b(a(n)^2) * a(2n+1) =
 *            b(a(n) + a(n-1)) * where b(k) = abs(k)*(-1)^k
 * Range:     i8 n <= 55, i16 n <= 111, i32 n <= 223, i64 n <= 447,
 *            i128 n <= 895, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* b(k) = abs(k)*(-1)^k */
static seq_t b_func(seq_t k)
{
    seq_t abs_k = (k >= 0) ? k : -k;
    /* parity: lowest bit indicates even/odd even for negative values */
    seq_t sign = (k & 1) ? (seq_t)-1 : (seq_t)1;
    return sign * abs_k;
}

/* ---------- cache storage ---------- */
#ifdef __cplusplus
static std::vector<seq_t> cache;
static std::vector<char> computed;
static bool cache_inited = false;
#else
static seq_t *cache      = NULL;
static unsigned char *computed = NULL;
static size_t cache_cap   = 0;
static int cache_inited   = 0;
#endif

/* enlarge cache when needed */
static void ensure_capacity(size_t n)
{
#ifdef __cplusplus
    if (!cache_inited) {
        cache.resize(2);
        computed.resize(2, 0);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        computed[0] = computed[1] = 1;
        cache_inited = true;
    }
    if (n < cache.size()) return;
    size_t new_cap = cache.size() ? cache.size() : 16;
    while (new_cap <= n) new_cap <<= 1;
    cache.resize(new_cap);
    computed.resize(new_cap, 0);
#else
    if (!cache_inited) {
        size_t init_sz = 2;
        cache      = (seq_t*)malloc(init_sz * sizeof(seq_t));
        computed   = (unsigned char*)malloc(init_sz * sizeof(unsigned char));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        computed[0] = computed[1] = 1;
        cache_cap = init_sz;
        cache_inited = 1;
    }
    if (n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp_cache   = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    unsigned char *tmp_comp = (unsigned char*)realloc(computed, new_cap * sizeof(unsigned char));
    cache   = tmp_cache;
    computed = tmp_comp;
    for (size_t i = cache_cap; i < new_cap; ++i) computed[i] = 0;
    cache_cap = new_cap;
#endif
}

/* recursive evaluation with memoisation */
static seq_t compute_a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_capacity(idx);

#ifdef __cplusplus
    if (computed[idx]) return cache[idx];
#else
    if (computed[idx]) return cache[idx];
#endif

    seq_t val;
    if ((idx & 1) == 0) {               /* even: n = 2*m */
        idx_t m = n >> 1;
        seq_t am = compute_a(m);
        val = b_func(am * am);
    } else {                            /* odd: n = 2*m + 1, m >= 1 */
        idx_t m = n >> 1;
        seq_t am  = compute_a(m);
        seq_t am1 = compute_a(m - 1);
        val = b_func(am + am1);
    }

#ifdef __cplusplus
    cache[idx] = val;
    computed[idx] = 1;
#else
    cache[idx] = val;
    computed[idx] = 1;
#endif

    return val;
}

/* public interface required by the build system */
seq_t a(idx_t n)
{
    return compute_a(n);
}

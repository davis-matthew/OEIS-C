/*
 * A262421  Number of (2+1) X (n+1) 0..1 arrays with each row divisible by 3
 *          and column not divisible by 3, read as a binary number with top
 *          and left being the most significant bits.
 * https://oeis.org/A262421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence of order 7 * a(1)=5, a(2)=4,
 *            a(3)=45, a(4)=114, a(5)=709, * a(6)=2892, a(7)=15293, *
 *            a(n)=5*a(n-1)+12*a(n-2)-60*a(n-3)-39*a(n-4)+195*a(n-5)+28*a(n-6)-140*a(n-7)
 *            * for n≥8
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if __cplusplus
#  include <vector>
#  include <cstddef>
#endif
#include <stdlib.h>

static void init_cache_once(void);

#if __cplusplus
/* ---------- C++ implementation using std::vector ---------- */
static std::vector<seq_t> cache; /* 1‑based; cache[0] is unused */

static void init_cache_once(void)
{
    if (!cache.empty()) return;
    cache.resize(8);                /* indices 0..7 */
    cache[0] = 0;                   /* unused */
    cache[1] = 5;
    cache[2] = 4;
    cache[3] = 45;
    cache[4] = 114;
    cache[5] = 709;
    cache[6] = 2892;
    cache[7] = 15293;
}

static void expand_cache(idx_t n)
{
    init_cache_once();
    if (static_cast<idx_t>(cache.size()) - 1 >= n) return; /* enough */

    /* compute new terms */
    for (idx_t i = static_cast<idx_t>(cache.size()); i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        pos += 5 * cache[i - 1];
        pos += 12 * cache[i - 2];
        pos += 195 * cache[i - 5];
        pos += 28 * cache[i - 6];
        neg += 60 * cache[i - 3];
        neg += 39 * cache[i - 4];
        neg += 140 * cache[i - 7];
        cache.push_back(pos - neg);
    }
}

#else
/* ---------- C implementation using malloc/realloc ---------- */
static seq_t *cache = NULL;      /* 1‑based; cache[0] unused */
static idx_t cache_len = 0;      /* current highest index cached (0 if none) */
static idx_t cache_cap = 0;      /* allocated capacity */

static void init_cache_once(void)
{
    if (cache_len) return;

    /* allocate at least 8 slots */
    cache_cap = 8;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);
    cache[0] = 0;
    cache[1] = 5;
    cache[2] = 4;
    cache[3] = 45;
    cache[4] = 114;
    cache[5] = 709;
    cache[6] = 2892;
    cache[7] = 15293;
    cache_len = 7;
}

static void expand_cache(idx_t n)
{
    init_cache_once();
    if (cache_len >= n) return;              /* already cached */

    /* grow capacity if necessary */
    if (cache_cap < n + 1) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new terms */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        pos += 5 * cache[i - 1];
        pos += 12 * cache[i - 2];
        pos += 195 * cache[i - 5];
        pos += 28 * cache[i - 6];
        neg += 60 * cache[i - 3];
        neg += 39 * cache[i - 4];
        neg += 140 * cache[i - 7];
        cache[i] = pos - neg;
    }
    cache_len = n;
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset ≥1 */
    expand_cache(n);
#if __cplusplus
    return cache[n];
#else
    return cache[n];
#endif
}

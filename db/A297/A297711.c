/*
 * A297711  Number of maximal irredundant sets in the n-dipyramidal graph.
 * https://oeis.org/A297711
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 2*a(n-1) - a(n-3) -
 *            a(n-6) + 2*a(n-9) + 2*a(n-10) - 4*a(n-11) + a(n-13) - a(n-14) +
 *            2*a(n-15) - a(n-16), with initial values a(3)=4, a(4)=15,
 *            a(5)=21, a(6)=24, a(7)=29, a(8)=31, a(9)=49, a(10)=83, a(11)=89,
 *            a(12)=124, a(13)=170, a(14)=241, a(15)=374, a(...
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 57, u64 n <= 115,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#include <stddef.h>
#endif

/* Pre‑computed values for indices 0 … 19 (0…2 unused). */
static const seq_t pre_values[20] = {
    0, 0, 0, /* 0,1,2 */
    4, 15, 21, 24, 29, 31, 49, 83, 89, 124, 170, 241,
    374, 511, 732, 1029, 1540
};

#ifdef __cplusplus
static std::vector<seq_t> cache;
/* Create cache and fill with the known initial values. */
static void init_cache(void)
{
    if (cache.empty()) {
        cache.resize(20);
        for (size_t i = 0; i < 20; ++i)
            cache[i] = pre_values[i];
    }
}
#else
static seq_t *cache = NULL;
static size_t cache_len = 0;
/* Create cache and fill with the known initial values. */
static void init_cache(void)
{
    if (cache == NULL) {
        cache_len = 20;
        cache = (seq_t*)calloc(cache_len, sizeof(seq_t));
        for (size_t i = 0; i < 20; ++i)
            cache[i] = pre_values[i];
    }
}
#endif

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;           /* values below the offset are undefined */

    init_cache();

#ifdef __cplusplus
    size_t N = (size_t)n;
    /* Compute any missing values up to N using the recurrence. */
    for (size_t i = cache.size(); i <= N; ++i) {
        seq_t val = 2*cache[i-1];
        val -= cache[i-3];
        val -= cache[i-6];
        val += 2*cache[i-9];
        val += 2*cache[i-10];
        val -= 4*cache[i-11];
        val += cache[i-13];
        val -= cache[i-14];
        val += 2*cache[i-15];
        val -= cache[i-16];
        cache.push_back(val);
    }
    return cache[N];
#else
    size_t N = (size_t)n;
    if (cache_len <= N) {
        /* Extend the cache. */
        seq_t *tmp = (seq_t*)realloc(cache, (N+1)*sizeof(seq_t));
        if (tmp == NULL)
            return 0;          /* allocation failed, return 0 as a graceful fail */
        cache = tmp;
        for (size_t i = cache_len; i <= N; ++i) {
            seq_t val = 2*cache[i-1];
            val -= cache[i-3];
            val -= cache[i-6];
            val += 2*cache[i-9];
            val += 2*cache[i-10];
            val -= 4*cache[i-11];
            val += cache[i-13];
            val -= cache[i-14];
            val += 2*cache[i-15];
            val -= cache[i-16];
            cache[i] = val;
        }
        cache_len = N + 1;
    }
    return cache[N];
#endif
}

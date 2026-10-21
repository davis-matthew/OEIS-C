/*
 * A289798  p-INVERT of (-1 + 2^n), where p(S) = 1 - S - S^2.
 * https://oeis.org/A289798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 15*a(n-2) + 14*a(n-3) - 4*a(n-4); initial
 *            values: a(0)=1, a(1)=5, a(2)=22, a(3)=93
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __cplusplus
#include <stdlib.h>

static seq_t cache[10001];
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    /* ensure initial terms are stored */
    if (!cache_len) {
        cache[0] = 1; cache[1] = 5; cache[2] = 22; cache[3] = 93;
        cache_len = 4;
    }

    /* use static array for indices < 10000 */
    if (n < 10000) {
        if (n < cache_len)
            return cache[n];

        while (cache_len <= n) {
            seq_t t = cache[cache_len-1] * 7;
            t += cache[cache_len-3] * 14;
            t -= cache[cache_len-2] * 15;
            t -= cache[cache_len-4] * 4;
            cache[cache_len++] = t;
        }
        return cache[n];
    }

    /* for larger indices, fall back to dynamic allocation */
    static seq_t *dyn = NULL;
    static size_t dyn_cap = 0, dyn_len = 0;
    if (!dyn) {
        dyn_cap = 8;
        dyn = (seq_t*)malloc(dyn_cap * sizeof(seq_t));
        if (!dyn) abort();
        dyn[0] = 1; dyn[1] = 5; dyn[2] = 22; dyn[3] = 93;
        dyn_len = 4;
    }
    if (n >= dyn_cap) {
        size_t new_cap = dyn_cap ? dyn_cap << 1 : 8;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(dyn, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        dyn = tmp;
        dyn_cap = new_cap;
    }
    while (dyn_len <= n) {
        seq_t t = dyn[dyn_len-1] * 7;
        t += dyn[dyn_len-3] * 14;
        t -= dyn[dyn_len-2] * 15;
        t -= dyn[dyn_len-4] * 4;
        dyn[dyn_len++] = t;
    }
    return dyn[n];
}
#else
#include <vector>

seq_t a(idx_t n)
{
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(4);
        cache[0] = 1; cache[1] = 5; cache[2] = 22; cache[3] = 93;
    }
    while (cache.size() <= n) {
        size_t m = cache.size();
        seq_t t = cache[m-1] * 7;
        t += cache[m-3] * 14;
        t -= cache[m-2] * 15;
        t -= cache[m-4] * 4;
        cache.push_back(t);
    }
    return cache[n];
}
#endif

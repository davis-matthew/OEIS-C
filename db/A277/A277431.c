/*
 * A277431  Expansion of e.g.f.: cosh(sqrt(2)*x)/(1-x).
 * https://oeis.org/A277431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + 2*a(n-2) - 2*(n-2)*a(n-3), with a(0)=1,
 *            a(1)=1, a(2)=4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector for memoisation */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
    }
    if (n < cache.size())
        return cache[n];

    for (idx_t i = cache.size(); i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)1;
        } else if (i == 2) {
            val = (seq_t)4;
        } else {
            seq_t i_seq = (seq_t)i;
            seq_t term1 = i_seq * cache[i - 1];
            seq_t term2 = (seq_t)2 * cache[i - 2];
            seq_t term3 = (seq_t)2 * ((i_seq - (seq_t)2)) * cache[i - 3];
            val = term1 + term2;
            val -= term3;   /* guaranteed non‑negative */
        }
        cache.push_back(val);
    }
    return cache[n];
#else
    /* C implementation using malloc/realloc for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
    }

    if (n < cache_size)
        return cache[n];

    /* Expand cache to hold up to index n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (new_cache == NULL) {
        /* allocation failed – in typical competitive settings this won't happen */
        return (seq_t)0;
    }
    cache = new_cache;

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)1;
        } else if (i == 2) {
            val = (seq_t)4;
        } else {
            seq_t i_seq = (seq_t)i;
            seq_t term1 = i_seq * cache[i - 1];
            seq_t term2 = (seq_t)2 * cache[i - 2];
            seq_t term3 = (seq_t)2 * ((i_seq - (seq_t)2)) * cache[i - 3];
            val = term1 + term2;
            val -= term3;   /* guaranteed non‑negative */
        }
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
#endif
}

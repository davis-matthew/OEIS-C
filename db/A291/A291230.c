/*
 * A291230  p-INVERT of (0,1,0,1,0,1,...), where p(S) = (1 - S)(1 - 2 S)(1 - 3
 *          S).
 * https://oeis.org/A291230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A291230: p‑INVERT of (0,1,0,1,0,1,…) with p(S)=(1‑S)(1‑2S)(1‑3S)
   Recurrence: a(n)=6*a(n‑1)-8*a(n‑2)-6*a(n‑3)+8*a(n‑4)+6*a(n‑5)+a(n‑6)
   Initial values: a(0)=6, a(1)=25, a(2)=96, a(3)=351, a(4)=1242, a(5)=4304, a(6)=14706 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++: use vector for convenient dynamic growth and proper construction */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(7);
        cache.push_back(6);    /* a(0) */
        cache.push_back(25);   /* a(1) */
        cache.push_back(96);   /* a(2) */
        cache.push_back(351);  /* a(3) */
        cache.push_back(1242); /* a(4) */
        cache.push_back(4304); /* a(5) */
        cache.push_back(14706);/* a(6) */
    }
    if (n < cache.size())
        return cache[(size_t)n];

    for (idx_t i = cache.size(); i <= n; ++i) {
        seq_t val = 0;
        val += (seq_t)6 * cache[(size_t)i-1];
        val -= (seq_t)8 * cache[(size_t)i-2];
        val -= (seq_t)6 * cache[(size_t)i-3];
        val += (seq_t)8 * cache[(size_t)i-4];
        val += (seq_t)6 * cache[(size_t)i-5];
        val += cache[(size_t)i-6];
        cache.push_back(val);
    }
    return cache[(size_t)n];
#else
    /* C: use malloc/realloc; cache holds the values sequentially */
    static seq_t *cache = NULL;
    static idx_t len = 0;
    static const seq_t init_vals[7] = {6, 25, 96, 351, 1242, 4304, 14706};

    if (len == 0) {        /* first call: reserve initial terms */
        len = 7;
        cache = (seq_t*)malloc(len * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        for (idx_t i = 0; i < 7; ++i)
            cache[i] = init_vals[i];
    }

    if (n < len)
        return cache[(size_t)n];

    /* Grow the cache to accommodate up to index n */
    seq_t *tmp = (seq_t*)realloc(cache, ((n + 1) * sizeof(seq_t)));
    if (!tmp) return (seq_t)0;
    cache = tmp;

    for (idx_t i = len; i <= n; ++i) {
        seq_t val = 0;
        val += (seq_t)6 * cache[(size_t)i-1];
        val -= (seq_t)8 * cache[(size_t)i-2];
        val -= (seq_t)6 * cache[(size_t)i-3];
        val += (seq_t)8 * cache[(size_t)i-4];
        val += (seq_t)6 * cache[(size_t)i-5];
        val += cache[(size_t)i-6];
        cache[(size_t)i] = val;
    }
    len = n + 1;
    return cache[(size_t)n];
#endif
}

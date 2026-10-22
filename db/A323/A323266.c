/*
 * A323266  A323265(n)/2.
 * https://oeis.org/A323266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) - 5*a(n-3) + 7*a(n-4) - a(n-5)
 *            a(1)=0, a(2)=1, a(3)=3, a(4)=13, a(5)=51
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 65,
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
    if (n == 0) return (seq_t)0;   /* undefined but safe */

#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.size() == 0) {
        cache.resize(6);               /* indices 0..5 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)51;
    }
    while (cache.size() <= n) {
        idx_t i = static_cast<idx_t>(cache.size());   /* next index to compute */
        seq_t val;
        if (i <= 5) {   /* already initialized, just pass through */
            val = (seq_t)0;
            if (i == 1) val = (seq_t)0;
            else if (i == 2) val = (seq_t)1;
            else if (i == 3) val = (seq_t)3;
            else if (i == 4) val = (seq_t)13;
            else if (i == 5) val = (seq_t)51;
        } else {
            val = (seq_t)5 * cache[i-1]
                - (seq_t)3 * cache[i-2]
                - (seq_t)5 * cache[i-3]
                + (seq_t)7 * cache[i-4]
                -         cache[i-5];
        }
        cache.push_back(val);
    }
    return cache[n];
#else
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* highest index computed */
    static idx_t cache_cap = 0;    /* allocated capacity */

    if (cache_cap <= n) {
        idx_t new_cap = n + 1;        /* need indices 0..n */
        seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();
        cache = new_ptr;
        cache_cap = new_cap;
    }

    if (cache_size == 0 && cache_cap > 0) {
        cache[0] = (seq_t)0;          /* unused */
    }

    while (cache_size < n) {
        idx_t i = cache_size + 1;
        seq_t val;
        if (i == 1) val = (seq_t)0;
        else if (i == 2) val = (seq_t)1;
        else if (i == 3) val = (seq_t)3;
        else if (i == 4) val = (seq_t)13;
        else if (i == 5) val = (seq_t)51;
        else {
            val = (seq_t)5 * cache[i-1]
                - (seq_t)3 * cache[i-2]
                - (seq_t)5 * cache[i-3]
                + (seq_t)7 * cache[i-4]
                -         cache[i-5];
        }
        cache[i] = val;
        cache_size = i;
    }
    return cache[n];
#endif
}

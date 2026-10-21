/*
 * A291337  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - 2 S - 2 S^3.
 * https://oeis.org/A291337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + 5*a(n-3), with a(0)=1, a(1)=3,
 *            a(2)=10, a(3)=34 for n ≥ 4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
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
    /* C++ implementation using std::vector for arbitrary‑precision types */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(64);
        cache.push_back((seq_t)1);
        cache.push_back((seq_t)3);
        cache.push_back((seq_t)10);
        cache.push_back((seq_t)34);
    }
    while (cache.size() <= n) {
        const size_t i = cache.size();
        seq_t t1 = ((seq_t)5) * cache[i - 1];
        seq_t t2 = ((seq_t)7) * cache[i - 2];
        seq_t t3 = ((seq_t)5) * cache[i - 3];
        cache.push_back(t1 + t3 - t2);
    }
    return cache[n];
#else
    /* C implementation using malloc/realloc */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;

    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)34;
        cache_len = 4;
    }

    if (n < cache_len)
        return cache[n];

    while (cache_len <= n) {
        const idx_t i = cache_len;
        seq_t t1 = ((seq_t)5) * cache[i - 1];
        seq_t t2 = ((seq_t)7) * cache[i - 2];
        seq_t t3 = ((seq_t)5) * cache[i - 3];
        seq_t term = t1 + t3 - t2;

        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[i] = term;
        cache_len++;
    }
    return cache[n];
#endif
}

/*
 * A351635  a(n) is the number of perfect matchings of an edge-labeled 2 X n
 *          Klein bottle grid graph, or equivalently the number of domino
 *          tilings of a 2 X n Klein bottle grid. (The twist is on the
 *          length-n side.)
 * https://oeis.org/A351635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + (n mod 2)*a(n-1) - 4*(n mod 2) with
 *            a(1) = 2, a(2) = 6 (This recurrence is valid for n >= 3.)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 38, u128 n <= 38,
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
    if (n == 0) return (seq_t)0;          /* out of range, safe return */

#ifdef __cplusplus
    /* C++ implementation using std::vector for automatic memory handling */
    static std::vector<seq_t> cache;
    if (cache.empty()) {                  /* initialise base cases once */
        cache.resize(3);                  /* indices 0 (unused), 1, 2 */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
    }

    /* grow cache up to required index */
    while (cache.size() <= (size_t)n) {
        size_t k = cache.size();          /* compute a(k) */
        seq_t val;
        if ((k & 1ULL) == 0) {            /* k even */
            val = cache[k - 1] + cache[k - 2];
        } else {                          /* k odd */
            val = cache[k - 1] + cache[k - 1] + cache[k - 2] - (seq_t)4;
        }
        cache.push_back(val);
    }
    return cache[(size_t)n];
#else
    /* C implementation using malloc/realloc */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated capacity (max index) */
    static size_t cache_len = 0;          /* next free index (max computed + 1) */

    /* ensure capacity for index n */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) { exit(1); }            /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base cases if not yet done */
    if (cache_len <= 2) {
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache_len = 3;                    /* next free index */
    }

    /* compute values up to n */
    while (cache_len <= (size_t)n) {
        size_t k = cache_len;              /* compute a(k) */
        seq_t val;
        if ((k & 1ULL) == 0) {             /* k even */
            val = cache[k - 1] + cache[k - 2];
        } else {                           /* k odd */
            val = cache[k - 1] + cache[k - 1] + cache[k - 2] - (seq_t)4;
        }
        cache[k] = val;
        ++cache_len;
    }
    return cache[(size_t)n];
#endif
}

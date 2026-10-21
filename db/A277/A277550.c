/*
 * A277550  Numbers k such that k/5^m == 1 (mod 5), where 5^m is the greatest
 *          power of 5 that divides k.
 * https://oeis.org/A277550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by incremental search. */
/* a(n) is the n‑th positive integer k such that k/5^m ≡ 1 (mod 5),
   where 5^m is the greatest power of 5 dividing k. */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

/* Helper to test whether a number satisfies the definition. */
static int good_k(seq_t k)
{
    while (k % 5 == 0) {
        k /= 5;
    }
    return (k % 5) == 1;
}

#ifdef __cplusplus
/* C++ build: use a std::vector for automatic construction of seq_t objects. */
static std::vector<seq_t> cache_vec;

/* Return the n-th term (1‑based). */
extern seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    /* If already cached, return cached value. */
    if (n <= cache_vec.size())
        return cache_vec[n - 1];

    /* Start searching from the next integer after the largest cached value. */
    seq_t cur = cache_vec.empty() ? 1 : cache_vec.back() + 1;

    /* Generate terms until we have at least n entries. */
    while (cache_vec.size() < n) {
        if (good_k(cur)) {
            cache_vec.push_back(cur);
        }
        ++cur;
    }
    return cache_vec[n - 1];
}

#else
/* C build: use a manually managed dynamic array. */
static seq_t *cache_arr = NULL;
static size_t   cache_cap = 0;
static size_t   cache_cnt = 0;

/* Ensure the cache has room for at least 'needed' entries. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;

    size_t new_cap = (cache_cap > 0) ? (cache_cap << 1) : 8;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(cache_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* out of memory */

    cache_arr = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term (1‑based). */
extern seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    /* If already cached, return cached value. */
    if (n <= cache_cnt)
        return cache_arr[n - 1];

    /* Start searching from the next integer after the largest cached value. */
    seq_t cur = (cache_cnt == 0) ? 1 : cache_arr[cache_cnt - 1] + 1;

    while (cache_cnt < n) {
        if (good_k(cur)) {
            ensure_cache(cache_cnt + 1);
            cache_arr[cache_cnt++] = cur;
        }
        ++cur;
    }
    return cache_arr[n - 1];
}
#endif

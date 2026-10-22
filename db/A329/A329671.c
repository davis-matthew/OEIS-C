/*
 * A329671  Number of excursions of length n with Motzkin-steps avoiding the
 *          consecutive steps UU, HH and DD.
 * https://oeis.org/A329671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 32, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A329671 – Motzkin excursion counts avoiding consecutive steps UU, HH, DD.
   Recurrence (n ≥ 5):
   (n+4)a(n) = (n+4)a(n‑1)+(n‑2)a(n‑2)+3n a(n‑3) – 6a(n‑4) – 4(n‑5)a(n‑5)
   Base values: a(0)=1, a(1)=1, a(2)=1, a(3)=3, a(4)=4.
*/

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* ---- C++ implementation with std::vector ---- */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(64);
        cache.resize(5);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)4;
    }
    if ((size_t)n < cache.size())
        return cache[(size_t)n];

    /* Extend cache up to n */
    for (size_t k = cache.size(); k <= (size_t)n; ++k) {
        seq_t term1 = (seq_t)(k + 4) * cache[k - 1];
        seq_t term2 = (seq_t)(k - 2) * cache[k - 2];
        seq_t term3 = (seq_t)(3 * k) * cache[k - 3];
        seq_t term4 = (seq_t)6 * cache[k - 4];
        seq_t term5 = (seq_t)(4 * (k - 5)) * cache[k - 5];
        seq_t numer = term1 + term2 + term3 - term4 - term5;
        cache.push_back(numer / (seq_t)(k + 4));
    }
    return cache[(size_t)n];
#else
    /* ---- C implementation with malloc/realloc ---- */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements allocated / valid */

    if (cache_size == 0) {
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();             /* abort if memory allocation fails */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)4;
    }

    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Grow the array to accommodate indices up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_size = new_size;

    /* Compute missing entries */
    for (size_t k = 5; k <= (size_t)n; ++k) {
        seq_t term1 = (seq_t)(k + 4) * cache[k - 1];
        seq_t term2 = (seq_t)(k - 2) * cache[k - 2];
        seq_t term3 = (seq_t)(3 * k) * cache[k - 3];
        seq_t term4 = (seq_t)6 * cache[k - 4];
        seq_t term5 = (seq_t)(4 * (k - 5)) * cache[k - 5];
        seq_t numer = term1 + term2 + term3 - term4 - term5;
        cache[k] = numer / (seq_t)(k + 4);
    }

    return cache[(size_t)n];
#endif
}

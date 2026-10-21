/*
 * A275864  Floor(r*a(n-1)) + floor(r*a(n-2)), where r = 5/3, a(0) = 1, a(1) =
 *          1.
 * https://oeis.org/A275864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((5/3)*a(n-1)) + floor((5/3)*a(n-2)), a(0)=1, a(1)=1
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 52, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */
#include <stdlib.h>  /* for malloc, realloc, NULL */

#if defined(__cplusplus)
#include <vector>
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* C++ implementation using std::vector for arbitrary‑precision seq_t */
    static std::vector<seq_t> cache;

    /* initialise */
    if (cache.empty()) {
        cache.reserve(2);
        cache.push_back((seq_t)1);  /* a(0) */
        cache.push_back((seq_t)1);  /* a(1) */
    }

    /* grow cache if necessary */
    if ((size_t)n >= cache.size()) {
        size_t old = cache.size();
        cache.resize((size_t)n + 1);
        const seq_t r_num = (seq_t)5;
        const seq_t r_den = (seq_t)3;
        for (size_t i = old; i <= (size_t)n; ++i) {
            seq_t t1 = (cache[i - 1] * r_num) / r_den;
            seq_t t2 = (cache[i - 2] * r_num) / r_den;
            cache[i] = t1 + t2;
        }
    }

    return cache[(size_t)n];
#else
    /* C implementation – seq_t is a plain integer type */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache for a(0) and a(1) */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure – unlikely */
        cache[0] = (seq_t)1;  /* a(0) */
        cache[1] = (seq_t)1;  /* a(1) */
        cache_len = 2;
    }

    /* grow cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */
        cache = tmp;
        const seq_t r_num = (seq_t)5;
        const seq_t r_den = (seq_t)3;
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t t1 = (cache[i - 1] * r_num) / r_den;
            seq_t t2 = (cache[i - 2] * r_num) / r_den;
            cache[i] = t1 + t2;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
#endif
}

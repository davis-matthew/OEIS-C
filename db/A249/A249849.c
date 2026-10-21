/*
 * A249849  Number of length 5+4 0..n arrays with no five consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A249849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A249849:
   Number of length 9 (5+4) arrays over {0..n} with no five consecutive terms
   having the maximum of any two terms equal to the minimum of the remaining
   three terms.
   Recurrence (n>10):
     a(n) = 10 a(n-1) - 45 a(n-2) + 120 a(n-3) - 210 a(n-4) + 252 a(n-5)
            - 210 a(n-6) + 120 a(n-7) - 45 a(n-8) + 10 a(n-9) - a(n-10)
   Initial values:
     a(1)=10, a(2)=1189, a(3)=30080, a(4)=337396, a(5)=2307418,
     a(6)=11342301, a(7)=44075760, a(8)=143723664, a(9)=409195002,
     a(10)=1045765501.
*/

#include <stdlib.h>

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 wid_t;  /* wider type for intermediate arithmetic */
static inline wid_t mulu128(seq_t a, unsigned int b) { return (wid_t)a * b; }
#else
/* fall back: use seq_t itself as no wider type available */
typedef seq_t wid_t;
static inline wid_t mulu(seq_t a, unsigned int b) { return a * (seq_t)b; }
#endif

/* Helper to get product using the widest available type */
static inline wid_t mulw(seq_t a, unsigned int b) {
#if defined(__SIZEOF_INT128__)
    return mulu128(a, b);
#else
    return mulu(a, b);
#endif
}

/* Initial seed values (indices 1..10). cache[0] is unused. */
static const seq_t seed[11] = {
    0ULL,                /* unused for index 0 */
    10ULL,
    1189ULL,
    30080ULL,
    337396ULL,
    2307418ULL,
    11342301ULL,
    44075760ULL,
    143723664ULL,
    409195002ULL,
    1045765501ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset, return 0 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initial allocation */
    if (cache == NULL) {
        /* allocate space for first 20 elements or up to n+1 if larger */
        cache_size = (n > 20 ? n + 1 : 21);
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed */
        for (idx_t i = 0; i < cache_size; ++i) cache[i] = 0;
    }

    /* grow cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* reallocation failed */
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }

    /* compute values up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0) continue;           /* already computed */

        if (i <= 10) {
            cache[i] = seed[i];
            continue;
        }

        wid_t pos = 0, neg = 0;
        pos += mulw(cache[i-1], 10U);
        pos += mulw(cache[i-3], 120U);
        pos += mulw(cache[i-5], 252U);
        pos += mulw(cache[i-7], 120U);
        pos += mulw(cache[i-9], 10U);

        neg += mulw(cache[i-2], 45U);
        neg += mulw(cache[i-4], 210U);
        neg += mulw(cache[i-6], 210U);
        neg += mulw(cache[i-8], 45U);
        neg += mulw(cache[i-10], 1U);

        /* cast back to seq_t (wrappers will truncate if overflow) */
        cache[i] = (seq_t)(pos - neg);
    }

    return cache[n];
}

/*
 * A251082  Number of (n+1) X (2+1) 0..2 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A251082: Number of (n+1)×(2+1) 0..2 arrays with no 2×2 subblock
 *   having the sum of its diagonal elements greater than the absolute
 *   difference of its antidiagonal elements.
 *
 * Empirical linear recurrence (valid for n > 8):
 *   a(n) =  6*a(n-1) -13*a(n-2) +10*a(n-3) + 5*a(n-4)
 *          -14*a(n-5) + 9*a(n-6) - 2*a(n-7)
 *
 * Initial values (1‑based):
 *   a(1)=83, a(2)=284, a(3)=616, a(4)=1306,
 *   a(5)=2534, a(6)=4848, a(7)=9086, a(8)=17102
 *
 * The implementation uses a memoised recurrence.  In C++ a
 * std::vector is used (because it handles arbitrary‑precision types
 * correctly); in C a hand‑rolled array with realloc is used.
 */

#include <stdlib.h>                 /* calloc, realloc, abort */
#ifdef __cplusplus
#include <vector>
#endif

/*--------------------------------------------------------------------*/
/* Helper: initialise the cache with the first eight terms            */
/*--------------------------------------------------------------------*/
#ifdef __cplusplus
static void init_cache(std::vector<seq_t> &v)
{
    if (v.empty()) {
        v.resize(9);                /* indices 0..8; 0 unused           */
        v[0] = (seq_t)0;
        v[1] = (seq_t)83;
        v[2] = (seq_t)284;
        v[3] = (seq_t)616;
        v[4] = (seq_t)1306;
        v[5] = (seq_t)2534;
        v[6] = (seq_t)4848;
        v[7] = (seq_t)9086;
        v[8] = (seq_t)17102;
    }
}
#else
static void init_cache(seq_t **p, size_t *size, size_t *cap)
{
    if (*p == NULL) {
        *cap = 32;                 /* initial capacity                */
        *size = 9;                 /* store 0..8                      */
        *p = (seq_t *)calloc(*cap, sizeof(seq_t));
        if (*p == NULL) abort();
        (*p)[0] = (seq_t)0;
        (*p)[1] = (seq_t)83;
        (*p)[2] = (seq_t)284;
        (*p)[3] = (seq_t)616;
        (*p)[4] = (seq_t)1306;
        (*p)[5] = (seq_t)2534;
        (*p)[6] = (seq_t)4848;
        (*p)[7] = (seq_t)9086;
        (*p)[8] = (seq_t)17102;
    }
}
#endif

/*--------------------------------------------------------------------*/
/* Public function                                                     */
/*--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined for n=0, return 0   */

#ifdef __cplusplus
    static std::vector<seq_t> cache;
    init_cache(cache);

    /* grow cache up to n */
    while ((idx_t)cache.size() <= n) {
        idx_t i  = (idx_t)cache.size();
        seq_t val = (seq_t)6   * cache[i-1]
                  - (seq_t)13 * cache[i-2]
                  + (seq_t)10 * cache[i-3]
                  + (seq_t)5  * cache[i-4]
                  - (seq_t)14 * cache[i-5]
                  + (seq_t)9  * cache[i-6]
                  - (seq_t)2  * cache[i-7];
        cache.push_back(val);
    }
    return cache[n];

#else
    static seq_t *cache  = NULL;    /* 1‑based array */
    static size_t cache_size = 0;   /* number of elements stored (including 0)   */
    static size_t cache_cap   = 0;  /* allocated capacity                      */

    init_cache(&cache, &cache_size, &cache_cap);

    /* grow cache up to n */
    while ((idx_t)cache_size <= n) {
        /* ensure capacity */
        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            void *tmp = realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = (seq_t *)tmp;
        }
        idx_t i  = (idx_t)cache_size;
        seq_t val = (seq_t)6   * cache[i-1]
                  - (seq_t)13 * cache[i-2]
                  + (seq_t)10 * cache[i-3]
                  + (seq_t)5  * cache[i-4]
                  - (seq_t)14 * cache[i-5]
                  + (seq_t)9  * cache[i-6]
                  - (seq_t)2  * cache[i-7];
        cache[i] = val;
        cache_size++;
    }
    return cache[n];
#endif
}

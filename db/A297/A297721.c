/*
 * A297721  Number of 2Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 2 or 4 neighboring 1s.
 * https://oeis.org/A297721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + 3*a(n-3) - 2*a(n-4) - 24*a(n-5) +
 *            24*a(n-6) (n ≥ 7) Initial values: a(1)=1, a(2)=10, a(3)=34,
 *            a(4)=83, a(5)=258, a(6)=865 The function caches computed values
 *            to answer queries in any order
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Cached values of the sequence: index 0 unused. */
static seq_t *cache_ptr   = NULL;   /* used when compiling as C */
static size_t cache_size  = 0;        /* current size (including index 0)  */
static idx_t  last_computed = 0;      /* largest n for which cache is filled */

#ifdef __cplusplus
/* In C++ we use a std::vector to let the compiler handle object lifetime. */
static std::vector<seq_t> cache_vec;
#endif

/* Ensure the cache can store indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
#ifdef __cplusplus
    if (cache_vec.size() >= (size_t)(n + 1))
        return;
    cache_vec.resize((size_t)(n + 1));
#else
    if (cache_size >= (size_t)(n + 1))
        return;
    /* realloc must return void*; cast to seq_t*.  The array holds trivial
       types in C builds, so no construction is required. */
    void *tmp = realloc(cache_ptr, sizeof(seq_t) * (size_t)(n + 1));
    if (!tmp)
        abort();
    cache_ptr = (seq_t*)tmp;
    cache_size = (size_t)(n + 1);
#endif
}

/* Initialize the base terms a(1) … a(6). */
static void init_base(idx_t n)
{
    if (last_computed >= 6)
        return; /* already initialized */
    ensure_cache(6);
#ifdef __cplusplus
    cache_vec[1] = (seq_t)1;
    cache_vec[2] = (seq_t)10;
    cache_vec[3] = (seq_t)34;
    cache_vec[4] = (seq_t)83;
    cache_vec[5] = (seq_t)258;
    cache_vec[6] = (seq_t)865;
#else
    cache_ptr[1] = (seq_t)1;
    cache_ptr[2] = (seq_t)10;
    cache_ptr[3] = (seq_t)34;
    cache_ptr[4] = (seq_t)83;
    cache_ptr[5] = (seq_t)258;
    cache_ptr[6] = (seq_t)865;
#endif
    last_computed = 6;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined below offset */

    ensure_cache(n);
    init_base(n);

    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t a1, a2, a3, a4, a5, a6;
#ifdef __cplusplus
            a1 = cache_vec[i - 1];
            a2 = cache_vec[i - 2];
            a3 = cache_vec[i - 3];
            a4 = cache_vec[i - 4];
            a5 = cache_vec[i - 5];
            a6 = cache_vec[i - 6];
#else
            a1 = cache_ptr[i - 1];
            a2 = cache_ptr[i - 2];
            a3 = cache_ptr[i - 3];
            a4 = cache_ptr[i - 4];
            a5 = cache_ptr[i - 5];
            a6 = cache_ptr[i - 6];
#endif
            seq_t val = (seq_t)4 * a1
                      - (seq_t)3 * a2
                      + (seq_t)3 * a3
                      - (seq_t)2 * a4
                      - (seq_t)24 * a5
                      + (seq_t)24 * a6;
#ifdef __cplusplus
            cache_vec[i] = val;
#else
            cache_ptr[i] = val;
#endif
        }
        last_computed = n;
    }

#ifdef __cplusplus
    return cache_vec[n];
#else
    return cache_ptr[n];
#endif
}

/*
 * A283893  Relative of Hofstadter Q-sequence: a(1) = 3, a(2) = 2; thereafter
 *          a(n) = a(n-a(n-1)) + a(n-a(n-2)).
 * https://oeis.org/A283893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)), with a(1)=3, a(2)=2. a(n)=0
 *            for n<=0. The function a(idx_t n) returns the nth term
 * Range:     u8 n <= 373, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
#include <vector>
#endif

#ifndef __cplusplus
/* C implementation – data stored in a raw array. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
#else
/* C++ implementation – data stored in a std::vector. */
static std::vector<seq_t> cache;
#endif

/* Highest index that has been computed so far. */
static idx_t computed = 0;

/* Ensure the storage can hold terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
#if !defined(__cplusplus)
    if (cache_cap > n) return;
    idx_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
    while (new_cap <= n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;

    /* Zero‑initialize the newly allocated part. */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        cache[i] = (seq_t)0;
    cache_cap = new_cap;
#else
    if (cache.size() > n) return;
    cache.resize(n + 1);   /* index 0 is automatically default‑constructed
                             (zero for the arithmetic types used). */
#endif
}

/* -------------------------------------------------------------------- */
/* Public sequence routine – works for any order of calls.              */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* convention for n<=0 */

    ensure_cache(n);

    for (idx_t i = computed + 1; i <= n; ++i)
    {
        if (i == 1)
        {
#if defined(__cplusplus)
            cache[1] = (seq_t)3;
#else
            cache[1] = (seq_t)3;
#endif
        }
        else if (i == 2)
        {
#if defined(__cplusplus)
            cache[2] = (seq_t)2;
#else
            cache[2] = (seq_t)2;
#endif
        }
        else
        {
            /* Obtain the indices for the recursive calls. */
            idx_t sub1 = (idx_t)cache[i - 1];
            idx_t idx1 = (sub1 <= i) ? (i - sub1) : 0;
            seq_t val1 = (idx1 > 0) ?
#if defined(__cplusplus)
                          cache[static_cast<size_t>(idx1)]
#else
                          cache[idx1]
#endif
                          : (seq_t)0;

            idx_t sub2 = (idx_t)cache[i - 2];
            idx_t idx2 = (sub2 <= i) ? (i - sub2) : 0;
            seq_t val2 = (idx2 > 0) ?
#if defined(__cplusplus)
                          cache[static_cast<size_t>(idx2)]
#else
                          cache[idx2]
#endif
                          : (seq_t)0;

            /* Compute a(i). */
#if defined(__cplusplus)
            cache[i] = val1 + val2;
#else
            cache[i] = val1 + val2;
#endif
        }
    }

    computed = n;
    return
#if defined(__cplusplus)
        cache[n];
#else
        cache[n];
#endif
}

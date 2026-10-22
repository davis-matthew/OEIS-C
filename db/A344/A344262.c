/*
 * A344262  a(0)=1; for n>0, a(n) = a(n-1)*n+1 if n is even, (a(n-1)+1)*n
 *          otherwise.
 * https://oeis.org/A344262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = a(n‑1)*n + 1 if n is even,
 *            (a(n‑1)+1)*n if n is odd
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <new>
#endif

/* static cache for memoization – works for both C and C++ builds */
static seq_t *cache = NULL;            /* array of computed values */
static size_t cache_cap = 0;           /* current number of allocated slots */
static idx_t computed_up_to = 0;       /* highest n that has been computed, 0 means a(0) not yet set */

/* Allocate or grow the cache array.  The implementation differs
   between C and C++ because C++ arbitrary‑precision types cannot
   be safely realigned with realloc() */
static void ensure_cache(idx_t needed)
{
#ifdef __cplusplus
    if ((size_t)needed < cache_cap) return;
    size_t new_cap = (size_t)needed + 1;
    seq_t* new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
    delete[] cache;
    cache = new_cache;
    cache_cap = new_cap;
#else
    if ((size_t)needed < cache_cap) return;
    size_t new_cap = (size_t)needed + 1;
    seq_t* new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();        /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
#endif
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    /* Ensure a(0) is initialised */
    ensure_cache(0);
    if (cache[0] == (seq_t)0)  /* first time only */
        cache[0] = (seq_t)1;
    if (n == 0) return cache[0];

    /* Ensure the cache can hold up to n */
    ensure_cache(n);

    /* Compute any missing terms up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if ((i & (idx_t)1) == 0) {               /* i even */
            cache[i] = cache[i - 1] * (seq_t)i + (seq_t)1;
        } else {                                 /* i odd */
            cache[i] = (cache[i - 1] + (seq_t)1) * (seq_t)i;
        }
    }
    computed_up_to = n;
    return cache[n];
}

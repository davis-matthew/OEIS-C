/*
 * A373171  a(1) = 1; for n >= 2, a(n) = noz(a(n-1) + 3*n - 2), where noz(n) =
 *          A004719(n).
 * https://oeis.org/A373171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = noz(a(n-1) + 3*n - 2), a(1) = 1 where noz(n) removes all
 *            decimal zero digits from n
 * Range:     u8 n <= 14, u16 n <= 1353, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   remove_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/remove_zeros.h"

#ifdef __cplusplus
/* For C++, use operator new/delete to allocate the array of seq_t,
   ensuring that constructors are called.  */
#define ALLOC(n) (new seq_t[(n)])
#define FREE(p)  (delete[] (p))
#else
/* For C, use malloc/free.  */
#define ALLOC(n) ((seq_t*)malloc((n) * sizeof(seq_t)))
#define FREE(p)  free((p))
#endif

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  last_computed = 0;

static void
ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;
    seq_t *new_cache = ALLOC(n);
    if (!new_cache)
        return; /* allocation failure – leave cache unchanged */
    /* Copy old values to the new cache.  */
    for (idx_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];
    if (cache)
        FREE(cache);
    cache = new_cache;
    cache_cap = n;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                 /* sequence is defined for n >= 1 */

    /* Make sure the cache can store n terms.  */
    ensure_cache(n);

    /* Initialise a(1) if it hasn't been computed yet.  */
    if (last_computed == 0) {
        cache[0] = 1;
        last_computed = 1;
    }

    /* Compute missing terms up to index n.  */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];                   /* a(i-1) */
        seq_t add  = (seq_t)(3 * (seq_t)i - 2);      /* 3*i-2 fits in seq_t */
        seq_t temp = prev + add;
        cache[i - 1] = remove_zeros(temp);           /* a(i) */
    }
    last_computed = n;

    return cache[n - 1];
}

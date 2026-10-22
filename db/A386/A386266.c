/*
 * A386266  G.f. A(x) satisfies A(x) = 1/( (1+x) * (1 - x*A(x) - 2*x^2*A'(x))
 *          ).
 * https://oeis.org/A386266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386266.
   Recurrence: a(n) = (-1)^n + n * Σ_{k=0}^{n-1} a(k) * a(n-1-k)
   Initial value: a(0) = 1
   The function is memoised and works for any build (C or C++),
   with seq_t being an integer type or an arbitrary‑precision class. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>   /* for memset in C */

#ifdef __cplusplus
#include <algorithm>  /* for std::copy */
#endif

/* Forward declaration of utility to allocate big‑int arrays if needed.
   The build provides alloc_seq_array, free_seq_array. */
extern seq_t *alloc_seq_array(size_t n);
extern void free_seq_array(seq_t *p);

/* Static cache of already computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;       /* already large enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= (size_t)n)
        new_cap *= 2;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    if (cache) {
        std::copy(cache, cache + cache_cap, tmp);
        delete[] cache;
    }
    /* New elements are default constructed (zero for built‑ins and for big‑ints). */
    cache = tmp;
#else
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();      /* out of memory */
    cache = tmp;
    /* Zero new part. */
    memset(cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
#endif
    cache_cap = new_cap;
}

/* Public function returning a(n). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (cache_cap == 0) {
        /* This should never happen, but guard against it. */
        cache = NULL;
        cache_cap = 0;
        return (seq_t)0;
    }

    /* Initialise the first term if needed. */
    if (cache[0] == (seq_t)0)
        cache[0] = (seq_t)1;     /* a(0) = 1 */

    /* Compute any missing terms up to n. */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0)
            continue;            /* already computed */

        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            sum += cache[k] * cache[i - 1 - k];
        }

        seq_t val = ( (i & 1) ? (seq_t)(-1) : (seq_t)1 );
        val += ((seq_t)i) * sum;
        cache[i] = val;
    }
    return cache[n];
}

/*
 * A292535  p-INVERT of the squares (A000290), where p(S) = 1 + S - 2 S^2.
 * https://oeis.org/A292535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 15*a(n-2) + 24*a(n-3) - 11*a(n-4) + 5*a(n-5) -
 *            a(n-6) Initial terms: a(0)=1, a(1)=7, a(2)=38, a(3)=189,
 *            a(4)=909, a(5)=4368, a(6)=21093 The sequence is computed
 *            iteratively and memoised so that calls can come in any order.
 *            All arithmetic is performed using the native seq_t type,...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache: cache[i] holds a(i) once computed.  Uninitialised
   entries are zero (the sequence never contains 0). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure that cache is large enough to hold indices up to n.  New
   entries are initialised to 0. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;                      /* already big enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < (size_t)(n + 1))
        new_cap = (size_t)(n + 1);

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) {
        abort();                     /* memory allocation failed */
    }

    /* Copy any existing values and free the old buffer. */
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    /* Initialise the newly allocated slots to zero. */
    for (size_t i = cache_cap; i <= (size_t)n; ++i)
        new_cache[i] = 0;

    cache    = new_cache;
    cache_cap = new_cap;
}

/* Public function returning a(n). */
seq_t a(idx_t n)
{
    /* The sequence never contains zero, so zero can be used as a
       sentinel for "not computed yet". */
    static const seq_t init[7] = {1, 7, 38, 189, 909, 4368, 21093};

    ensure_cache(n);

    /* Initialise the first seven terms if they are not yet set. */
    for (idx_t i = 0; i <= 6 && i <= n; ++i) {
        if (cache[i] == 0)
            cache[i] = init[i];
    }

    /* Compute all needed terms up to n. */
    for (idx_t i = 7; i <= n; ++i) {
        if (cache[i] != 0)          /* already computed in a previous call */
            continue;

        /* Use the recurrence: a(i) = 7*a(i-1) - 15*a(i-2) + 24*a(i-3) 
           - 11*a(i-4) + 5*a(i-5) - a(i-6) */
        seq_t pos = 7 * cache[i - 1] + 24 * cache[i - 3] + 5 * cache[i - 5];
        seq_t neg = 15 * cache[i - 2] + 11 * cache[i - 4] + cache[i - 6];
        cache[i] = pos - neg;        /* pos >= neg in this sequence */
    }

    return cache[n];
}

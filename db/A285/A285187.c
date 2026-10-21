/*
 * A285187  a(n) = Sum(psi(k-1)*psi(n-k-1),k=0..n)+(1-(-1)^n)/2, where psi(k)
 *          = A000931(k+6).
 * https://oeis.org/A285187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum(psi(k-1)*psi(n-k-1),k=0..n)+(1-(-1)^n)/2. Using the
 *            linear recurrence: a(n) = 3*a(n-2) + 2*a(n-3) - 3*a(n-4) -
 *            4*a(n-5) + 2*a(n-7) + a(n-8) for n > 7, with initial terms
 *            a(0)=1, a(1)=3, a(2)=3, a(3)=7, a(4)=9, a(5)=15, a(6)=22,
 *            a(7)=33
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for indices 0..7. */
    static const seq_t init[8] = {1, 3, 3, 7, 9, 15, 22, 33};

    /* Static cache for computed values. */
    static seq_t *cache      = NULL;   /* pointer to stored a(i) */
    static size_t cache_cap  = 0;      /* capacity of the cache array */
    static idx_t   cached_n  = 7;      /* highest index that has been computed */

    /* Allocate or grow cache as required. */
    if (cache == NULL) {
        /* Start with capacity for at least 8 elements. */
        cache_cap = 8;
        if (cache_cap < (size_t)(n + 1))
            cache_cap = n + 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        /* Initialize the first eight terms. */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
    } else if (cache_cap <= (size_t)n) {
        /* Double the capacity until it can hold n+1 elements. */
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap = new_cap * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Out of memory; abort as this is a fatal error for the build. */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If n is within the precomputed range, return it directly. */
    if (n <= cached_n)
        return cache[n];

    /* Compute new values up to n using the recurrence. */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* Positive part: 3*a(i-2) + 2*a(i-3) + 2*a(i-7) + a(i-8) */
        seq_t pos = cache[i - 2] * 3
                  + cache[i - 3] * 2
                  + cache[i - 7] * 2
                  + cache[i - 8];

        /* Negative part: 3*a(i-4) + 4*a(i-5) */
        seq_t neg = cache[i - 4] * 3
                  + cache[i - 5] * 4;

        /* Result (positive part minus negative part). */
        cache[i] = pos - neg;
    }

    /* Update the highest computed index. */
    cached_n = n;
    return cache[n];
}

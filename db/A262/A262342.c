/*
 * A262342  Area of Lewis Carroll's paradoxical F(2n+1) X F(2n+3) rectangle.
 * https://oeis.org/A262342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2n+1) * Fibonacci(2n+3) = Fibonacci(2n+2)^2 + 1
 *            (n > 0)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the linear recurrence
   a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3) with
   a(1)=10, a(2)=65, a(3)=442.  */

#include <stdlib.h>

/* Cache for already computed values (1‑based indexing). */
static seq_t *cache = NULL;       /* array of terms */
static idx_t cache_size = 0;      /* highest index computed; 0 means none */
static size_t cache_cap = 0;      /* current allocated capacity (size_t) */

/* Ensure the cache has space for at least 'needed' indices (1‑based). */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= (size_t)needed)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < (size_t)needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();            /* fatal if memory cannot be allocated */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialize cache with the first three terms. */
static void init_cache(void)
{
    if (cache_size > 0)
        return;                            /* already initialized */
    /* Need indices 0..3; we ignore index 0. */
    ensure_capacity(4);
    cache[1] = (seq_t)10;                  /* a(1) */
    cache[2] = (seq_t)65;                  /* a(2) */
    cache[3] = (seq_t)442;                 /* a(3) */
    cache_size = 3;
}

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* undefined for n<1, return 0 */

    init_cache();

    if (n <= cache_size)
        return cache[n];

    /* Extend cache up to n. */
    ensure_capacity(n + 1);                /* indices 0..n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* Recurrence: a(i) = 8*a(i-1) - 8*a(i-2) + a(i-3) */
        cache[i] =
            (seq_t)8 * cache[i-1]
            - (seq_t)8 * cache[i-2]
            + cache[i-3];
    }
    cache_size = n;
    return cache[n];
}

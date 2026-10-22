/*
 * A373640  a(n) = Sum_{k=0..floor(n/3)} binomial(3*n-8*k,k).
 * https://oeis.org/A373640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(3*n-8*k,k) Recurrence:
 *            a(n) = a(n-1) + 3*a(n-3) - 3*a(n-6) + a(n-9) for n>8
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* static cache of computed terms */
static seq_t *cache = NULL;      /* array of sequence values      */
static size_t  cache_size = 0;   /* current allocated size        */

/* ensure that cache has space for indices 0..n */
static void ensure_cache(idx_t n)
{
    size_t idx = (size_t)n;
    if (idx < cache_size) return;               /* already large enough */

    size_t new_size = cache_size ? cache_size : 1;
    while (new_size <= idx) new_size <<= 1;      /* double until big enough */

    seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
    if (!new_cache) abort();                     /* allocation failure */

    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    /* zero the newly added slots */
    memset(new_cache + cache_size, 0,
           (new_size - cache_size) * sizeof(seq_t));

    cache     = new_cache;
    cache_size = new_size;
}

/* initialise the base cases 0..8 once */
static void init_base(void)
{
    /* at least 9 slots */
    ensure_cache(8);

    /* initial terms of the sequence */
    cache[0] = 1;
    cache[1] = 1;
    cache[2] = 1;
    cache[3] = 2;
    cache[4] = 5;
    cache[5] = 8;
    cache[6] = 12;
    cache[7] = 24;
    cache[8] = 45;
}

/* compute term a(n) */
seq_t a(idx_t n)
{
    init_base();                     /* ensure base values are set  */
    ensure_cache(n);                 /* make sure cache large enough */

    if (n <= 8) return cache[(size_t)n];

    /* compute all terms up to n using the recurrence */
    for (size_t i = 9; i <= (size_t)n; ++i) {
        if (!cache[i]) {              /* already computed? skip    */
            seq_t diff = cache[i - 3];
            diff -= cache[i - 6];     /* a[i-3] >= a[i-6] for i>8  */

            cache[i] = cache[i - 1] + cache[i - 9] + (diff * 3);
        }
    }
    return cache[(size_t)n];
}

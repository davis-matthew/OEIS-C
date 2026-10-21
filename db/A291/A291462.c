/*
 * A291462  a(n) = (1/2)*A291417(n).
 * https://oeis.org/A291462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - 4*a(n-3) - 2*a(n-4) (n >= 5)
 *            Initial values: a(0)=2, a(1)=9, a(2)=38, a(3)=161, a(4)=680
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;

/* Initialise the cache with the first five terms.  This is called
   automatically on the first request. */
static void
init_cache(void)
{
    if (cache != NULL)
        return;

    /* Allocate space for five initial terms */
    cache_len = 5;
    cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
    if (!cache)
        exit(1);                     /* fatal allocation failure */

    /* Store the initial terms (cast to avoid compiler truncation) */
    cache[0] = (seq_t)2ULL;
    cache[1] = (seq_t)9ULL;
    cache[2] = (seq_t)38ULL;
    cache[3] = (seq_t)161ULL;
    cache[4] = (seq_t)680ULL;
}

/* Grow the cache so it contains at least indices 0..n inclusive. */
static void
grow_cache(idx_t n)
{
    if (n < cache_len)
        return;                      /* already sufficient */

    /* Double the size until it covers n */
    idx_t new_len = cache_len;
    if (new_len == 0)
        new_len = 5;
    while (new_len <= n)
        new_len = new_len * 2 + 1;   /* extra +1 to avoid infinite loop */

    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        exit(1);                     /* fatal allocation failure */
    cache = tmp;

    /* Compute new terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];

        /* All arithmetic is in the type of the array elements.
           For unsigned types the recurrence guarantees that the
           subtraction will not underflow because the sequence is
           strictly increasing. */
        cache[i] = a1 * 4 + a2 * 2 - a3 * 4 - a4 * 2;
    }

    cache_len = n + 1;
}

/* Public sequence function */
seq_t
a(idx_t n)
{
    init_cache();          /* ensure cache exists */
    grow_cache(n);         /* extend cache if necessary */

    return cache[n];
}

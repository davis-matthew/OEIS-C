/*
 * A254685  Number of partially ordered partitions of n into parts less than
 *          or equal to 3, in which the order of adjacent 2's and 3's is
 *          unimportant.
 * https://oeis.org/A254685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-5) with initial values
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=7, a(5)=12
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache       = NULL;        /* dynamically grown array of values   */
static idx_t  cache_limit = 0;           /* number of slots currently allocated */
static idx_t  max_computed = (idx_t)-1;  /* highest index that has a value       */
static int    initialized  = 0;          /* true after seed values are written   */

/* Ensure the cache can hold indices 0..n.  Uses realloc.  */
static void ensure_cache(idx_t n)
{
    if (cache_limit > n)
        return;

    idx_t new_limit = cache_limit ? cache_limit : 16;
    while (new_limit <= n)
        new_limit <<= 1;                 /* grow by a factor of two */

    seq_t *tmp = (seq_t *)realloc(cache, new_limit * sizeof(seq_t));
    if (!tmp)  /* realloc failed */
        abort();

    cache       = tmp;
    cache_limit = new_limit;
}

/* Initialise the first six terms once.  */
static void init_cache(void)
{
    if (initialized)
        return;

    ensure_cache(5);                     /* need indices 0..5 */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)4;
    cache[4] = (seq_t)7;
    cache[5] = (seq_t)12;

    max_computed = 5;
    initialized  = 1;
}

/* Return a(n), memoising results for future calls.  */
seq_t a(idx_t n)
{
    init_cache();

    if (n <= 5)
        return cache[n];

    ensure_cache(n);

    for (idx_t i = max_computed + 1; i <= n; ++i)
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] - cache[i-5];

    max_computed = n;
    return cache[n];
}

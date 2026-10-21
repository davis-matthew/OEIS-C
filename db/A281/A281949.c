/*
 * A281949  Number of nX2 0..1 arrays with no element unequal to more than
 *          four of its king-move neighbors and with new values introduced in
 *          order 0 sequentially upwards.
 * https://oeis.org/A281949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + 4*a(n-3) * with a(1)=2, a(2)=8,
 *            a(3)=30 (offset 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static memoisation cache */
static seq_t *cache = NULL;          /* cache[n] holds a(n) for n>=1 */
static idx_t cache_size = 0;         /* largest n for which cache[n] is valid */

/* Ensure cache contains values up to and including n */
static void
ensure_cache(idx_t n)
{
    if (cache_size >= n)
        return;                     /* already computed */

    idx_t new_size = n;
    /* Allocate space for indices 0..new_size (index 0 unused) */
    seq_t *new_cache = (seq_t *)realloc(cache,
                                       (size_t)(new_size + 1) * sizeof(seq_t));
    if (!new_cache)
        exit(1);                    /* allocation failed */
    cache = new_cache;

    /* Initialise base cases if they do not yet exist */
    if (new_size >= 1)
        cache[1] = (seq_t)2;
    if (new_size >= 2)
        cache[2] = (seq_t)8;
    if (new_size >= 3)
        cache[3] = (seq_t)30;

    /* Compute remaining terms up to new_size */
    idx_t start = cache_size + 1;
    if (start < 4) start = 4;
    for (idx_t i = start; i <= new_size; ++i) {
        cache[i] = ((seq_t)4 * cache[i-1]) -
                   ((seq_t)2 * cache[i-2]) +
                   ((seq_t)4 * cache[i-3]);
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined for n<1, return 0 */
    ensure_cache(n);
    return cache[n];
}

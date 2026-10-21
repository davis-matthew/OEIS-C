/*
 * A284177  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 809", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A284177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 256·a(n-8) – 256·a(n-10) for n > 14 * Special
 *            case: a(17) = a(15) + 256·a(9) – 256·a(7) + 512 * Initial
 *            values: a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=5, a(5)=0, *
 *            a(6)=21, a(7)=8, a(8)=73, a(9)=8, a(10)=329, a(11)=136, *
 *            a(12)=1161, a(13)=168, a(14)=5289
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache array for computed terms */
static idx_t cache_size = 0;         /* allocated array size (in elements) */
static idx_t cache_computed = 0;     /* next index to compute (exclusive) */

/* ensure that cache can hold up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    idx_t new_size = cache_size ? cache_size : 16;
    while (new_size <= n)
        new_size <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = new_cache;
    cache_size = new_size;
}

/* Compute OEIS sequence A284177 */
seq_t a(idx_t n)
{
    /* allocate/resize cache if necessary */
    ensure_cache(n);

    /* Initialize the first 15 terms once */
    if (cache_computed == 0)
    {
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)0;
        cache[2]  = (seq_t)1;
        cache[3]  = (seq_t)0;
        cache[4]  = (seq_t)5;
        cache[5]  = (seq_t)0;
        cache[6]  = (seq_t)21;
        cache[7]  = (seq_t)8;
        cache[8]  = (seq_t)73;
        cache[9]  = (seq_t)8;
        cache[10] = (seq_t)329;
        cache[11] = (seq_t)136;
        cache[12] = (seq_t)1161;
        cache[13] = (seq_t)168;
        cache[14] = (seq_t)5289;
        cache_computed = 15;          /* indices 0..14 are ready */
    }

    /* If the requested index is already computed, return it */
    if (n < cache_computed)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = cache_computed; i <= n; ++i)
    {
        if (i <= 14)                /* safety: already stored */
            continue;

        seq_t term = cache[i - 2];
        term += (seq_t)256 * cache[i - 8];
        term -= (seq_t)256 * cache[i - 10];

        if (i == 17)                /* special numerator term for x^17 */
            term += (seq_t)512;

        cache[i] = term;
    }

    cache_computed = n + 1;
    return cache[n];
}

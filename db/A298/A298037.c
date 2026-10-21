/*
 * A298037  Partial sums of A298036.
 * https://oeis.org/A298037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=13, a(2)=25, a(3)=61, a(4)=85; a(n) = a(n-1) +
 *            2*a(n-2) - 2*a(n-3) - a(n-4) + a(n-5) for n ≥ 5
 * Range:     u8 n <= 6, u16 n <= 120, u32 n <= 9463, u64 n <= 9463,
 *            u128 n <= 9463, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* seq_t and idx_t are supplied by the build system */
static seq_t *cache = NULL;          /* stored partial sums   */
static size_t cache_size = 0;        /* number of elements stored */

static const seq_t init[5] = { (seq_t)1,
                               (seq_t)13,
                               (seq_t)25,
                               (seq_t)61,
                               (seq_t)85 };

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n;
    if (needed < cache_size)
        return;                   /* already available */

    size_t new_size = needed + 1; /* at least n+1 elements needed */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();                  /* allocation failed */

    cache = tmp;

    /* Compute new entries. */
    for (size_t i = cache_size; i < new_size; ++i) {
        if (i < 5) {
            cache[i] = init[i];
        } else {
            cache[i] = cache[i-1]
                     + (seq_t)2 * cache[i-2]
                     - (seq_t)2 * cache[i-3]
                     -            cache[i-4]
                     +            cache[i-5];
        }
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}

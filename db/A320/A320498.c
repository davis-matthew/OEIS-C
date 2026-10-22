/*
 * A320498  Coordination sequence of thinnest 5-neighbor packing of the plane
 *          with congruent hexagons with respect to a point of type D.
 * https://oeis.org/A320498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) * + a(n-16)
 *            - a(n-17) + a(n-18) - a(n-19) + a(n-20) - a(n-21) * initial
 *            values: * a(0)=1, a(1)=4, a(2)=8, a(3)=12, a(4)=13, a(5)=17,
 *            a(6)=23, * a(7)=28, a(8)=32, a(9)=35, a(10)=37, a(11)=42,
 *            a(12)=48, * a(13)=52, a(14)=53, a(15)=57, a(16)=62, a(...
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Initial terms for n = 0..24 */
static const seq_t init[25] = {
    1, 4, 8, 12, 13, 17, 23, 28, 32, 35,
    37, 42, 48, 52, 53, 57, 62, 69, 72, 74,
    75, 81, 87, 93, 95
};

/* Cache of computed values; cache[i] = a(i). */
static seq_t *cache = NULL;
static size_t cache_size = 0; /* number of elements stored (next index to fill) */

/* Compute a(n) for n >= 25 using the recurrence.  All required
 * previous terms are guaranteed to be in the cache. */
static seq_t recurrence(idx_t n)
{
    seq_t pos = cache[n-1];
    pos += cache[n-3];
    pos += cache[n-5];
    pos += cache[n-16];
    pos += cache[n-18];
    pos += cache[n-20];

    seq_t neg = cache[n-2];
    neg += cache[n-4];
    neg += cache[n-17];
    neg += cache[n-19];
    neg += cache[n-21];

    return pos - neg;
}

/* Expand the cache so that it contains terms up to and including n. */
static void ensure_upto(idx_t n)
{
    if ((size_t)n < cache_size)
        return;                 /* already available */

    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) {
        abort();                /* allocation failed */
    }
    cache = tmp;

    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        if (i < 25)
            cache[i] = init[(size_t)i];
        else
            cache[i] = recurrence(i);
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_upto(n);
    return cache[n];
}

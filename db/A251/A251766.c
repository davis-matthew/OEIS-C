/*
 * A251766  10-step Fibonacci sequence starting with 0,1,0,0,0,0,0,0,0,0.
 * https://oeis.org/A251766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+...+a(n-10), a(0)=0, a(1)=1,
 *            a(2)=...=a(9)=0
 * Range:     u8 n <= 18, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


static seq_t *cache = NULL;      /* cache of computed terms */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static size_t stored = 0;        /* number of terms that are currently stored */

/* Ensure that indices up to n are addressable in cache. */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)(n + 1);          /* indices 0 .. n */
    if (need <= cache_cap) return;         /* already enough space */

    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                       /* allocation failed – leave as is */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first ten terms of the sequence. */
static void init_base(void)
{
    if (stored) return;                     /* already initialised */

    /* Make sure we have space for indices 0..9. */
    ensure_capacity(9);

    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;
    for (size_t i = 2; i < 10; ++i)
        cache[i] = (seq_t)0;

    stored = 10;
}

seq_t a(idx_t n)
{
    /* On first call set up base terms. */
    if (stored == 0) init_base();

    if ((size_t)n < stored)
        return cache[(size_t)n];

    /* Make sure there is space for up to n. */
    ensure_capacity(n);

    /* In case base terms were lost, re‑initialise. */
    if (stored < 10) init_base();

    for (size_t i = stored; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t j = 0; j < 10; ++j)
            sum += cache[i - 10 + j];
        cache[i] = sum;
    }
    stored = (size_t)n + 1;
    return cache[(size_t)n];
}

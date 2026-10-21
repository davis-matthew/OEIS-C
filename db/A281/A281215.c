/*
 * A281215  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 342", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A281215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 10000*a(n-4) - 10000*a(n-6) a(0)=1, a(1)=11,
 *            a(2)=1, a(3)=111, a(4)=10001, a(5)=110111 (OEIS A281215 – binary
 *            x‑axis of Rule 342 cellular automaton)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_cache = (seq_t*)malloc(sizeof(seq_t) * new_cap);
    if (!new_cache)
        exit(EXIT_FAILURE); /* memory allocation failed */
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];
    if (cache)
        free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

static const seq_t init[6] = {
    (seq_t)1,
    (seq_t)11,
    (seq_t)1,
    (seq_t)111,
    (seq_t)10001,
    (seq_t)110111
};

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    if (idx >= cache_size)
    {
        grow_cache(idx + 1);
        for (size_t i = cache_size; i <= idx; ++i)
        {
            if (i < 6)
            {
                cache[i] = init[i];
            }
            else
            {
                seq_t val1 = cache[i - 2];
                seq_t mul2  = cache[i - 4] * (seq_t)10000;
                seq_t mul3  = cache[i - 6] * (seq_t)10000;
                cache[i] = val1 + mul2 - mul3;
            }
        }
        cache_size = idx + 1;
    }
    return cache[n];
}

/*
 * A294768  Number of permutations of [n] avoiding {2341, 3142, 4123}.
 * https://oeis.org/A294768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is computed by the linear recurrence
 *            a(n)=9*a(n-1)-33*a(n-2)+66*a(n-3)-78*a(n-4)+55*a(n-5)-23*a(n-6)+4*a(n-7),
 *            with initial values a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=21,
 *            a(5)=74, a(6)=253, a(7)=843. The recurrence holds for n≥8.
 *            Values are cached so that subsequent calls to a() run in O(1)...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *seq_cache = NULL;
static idx_t cache_size = 0;

/* extend the cache to at least (n+1) elements */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_size)
        return;

    /* resize */
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – abort */
        exit(EXIT_FAILURE);
    }
    seq_cache = new_cache;

    /* initialise new elements */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)         seq_cache[i] = 1;
        else if (i == 1)    seq_cache[i] = 1;
        else if (i == 2)    seq_cache[i] = 2;
        else if (i == 3)    seq_cache[i] = 6;
        else if (i == 4)    seq_cache[i] = 21;
        else if (i == 5)    seq_cache[i] = 74;
        else if (i == 6)    seq_cache[i] = 253;
        else if (i == 7)    seq_cache[i] = 843;
        else {
            /* compute using the recurrence with non‑negative intermediates */
            seq_t pos = 9 * seq_cache[i - 1];
            pos += 66 * seq_cache[i - 3];
            pos += 55 * seq_cache[i - 5];
            pos += 4 * seq_cache[i - 7];

            seq_t neg = 33 * seq_cache[i - 2];
            neg += 78 * seq_cache[i - 4];
            neg += 23 * seq_cache[i - 6];

            seq_cache[i] = pos - neg;
        }
    }

    cache_size = new_cap;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    return seq_cache[n];
}

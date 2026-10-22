/*
 * A335819  E.g.f.: exp((3/2) * x * (2 + x)).
 * https://oeis.org/A335819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * (a(n-1) + (n-1) * a(n-2)), a(0)=1, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed = 0;

    /* initial cache allocation */
    if (cache == NULL) {
        size_t init_cap = 2;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cap = init_cap;
        computed = 2;
    }

    /* grow cache if necessary */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap;
        while ((idx_t)new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms */
    while (computed <= n) {
        seq_t prev   = cache[computed - 1];
        seq_t prev2  = cache[computed - 2];
        seq_t term   = (seq_t)3 * (prev + (seq_t)(computed - 1) * prev2);
        cache[computed] = term;
        ++computed;
    }

    return cache[n];
}

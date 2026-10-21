/*
 * A244472  2nd-largest term in n-th row of Stern's diatomic triangle A002487.
 * https://oeis.org/A244472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) for n>5 * Initial values: a(1)=1, a(2)=2,
 *            a(3)=4, a(4)=7, a(5)=12 * (The recurrence equivalently expresses
 *            the generating function * x*(1 + x + x^2 + x^3 + x^4)/(1 - x -
 *            x^2).)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;  /* number of allocated elements */
    static const seq_t init_vals[5] = {1, 2, 4, 7, 12};

    /* one‑based to zero‑based conversion */
    size_t idx = (size_t)(n - 1);

    /* ensure cache is initialised */
    if (capacity == 0) {
        capacity = 5;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;      /* out of memory – graceful failure */
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init_vals[i];
    }

    /* extend cache if needed */
    if (idx >= capacity) {
        size_t new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;       /* out of memory – graceful failure */
        cache = tmp;
        /* compute missing terms */
        for (size_t i = capacity; i <= idx; ++i) {
            /* i>=5 holds because initial 5 terms exist */
            cache[i] = cache[i - 1] + cache[i - 2];
        }
        capacity = new_cap;
    }

    return cache[idx];
}

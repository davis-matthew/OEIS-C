/*
 * A319974  Expansion of (1-x^3+x^4+x^6)/(b(2)*b(3)*b(4)) where b(n) = 1-x^n.
 * https://oeis.org/A319974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) - a(n-5) - a(n-6) - a(n-7) +
 *            a(n-9) (n>8) with initial values
 *            a(0)=1,a(1)=0,a(2)=1,a(3)=0,a(4)=3,a(5)=0,a(6)=4,a(7)=1,a(8)=6
 * Range:     u8 n <= 75, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t capacity = 0;
static idx_t max_computed = 0;
static const seq_t init_vals[9] = {1,0,1,0,3,0,4,1,6};

seq_t a(idx_t n)
{
    /* Lazy initialization of the cache */
    if (cache == NULL) {
        capacity = 9;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure, fallback to 0 */
        for (size_t i = 0; i < 9; ++i) cache[i] = init_vals[i];
        max_computed = 8;
    }

    if (n <= max_computed) {
        return cache[n];
    }

    /* Grow capacity if needed */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* i >= 9 by construction */
        seq_t add = cache[(size_t)(i - 2)] + cache[(size_t)(i - 3)]
                  + cache[(size_t)(i - 4)] + cache[(size_t)(i - 9)];
        seq_t sub = cache[(size_t)(i - 5)] + cache[(size_t)(i - 6)]
                  + cache[(size_t)(i - 7)];
        cache[(size_t)i] = add - sub;
    }
    max_computed = n;
    return cache[n];
}

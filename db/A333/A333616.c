/*
 * A333616  Expansion of x*(1 + 2*x + x^2 - 4*x^3 - x^4 + 2*x^5)/((1 - x)^3*(1
 *          + x)^2).
 * https://oeis.org/A333616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+2*a(n-2)-2*a(n-3)-a(n-4)+a(n-5) for n>=7, with
 *            a(0)=0,a(1)=1,a(2)=3,a(3)=6,a(4)=6,a(5)=10,a(6)=10
 * Range:     u8 n <= 42, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array holding a[0..max_calc]        */
    static size_t cap   = 0;             /* allocated capacity in elements       */
    static idx_t  max_calc = 0;          /* highest index computed so far        */

    /* Initial values for n<7, no caching needed. */
    if (n < 7) {
        static const seq_t init[7] = { (seq_t)0, (seq_t)1, (seq_t)3,
                                      (seq_t)6, (seq_t)6, (seq_t)10,
                                      (seq_t)10 };
        return init[n];
    }

    /* If the value is already cached, return it immediately. */
    if (n <= max_calc) {
        return cache[n];
    }

    /* Ensure the cache has space for indices up to n. */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first seven entries if they are not yet set. */
    if (max_calc < 6) {
        static const seq_t init[7] = { (seq_t)0, (seq_t)1, (seq_t)3,
                                      (seq_t)6, (seq_t)6, (seq_t)10,
                                      (seq_t)10 };
        for (size_t i = 0; i < 7; ++i) {
            cache[i] = init[i];
        }
        max_calc = 6;
    }

    /* Compute values from max_calc+1 up to n using the linear recurrence. */
    for (idx_t i = max_calc + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        cache[i] = a1 + (seq_t)2 * a2 - (seq_t)2 * a3 - a4 + a5;
    }

    max_calc = n;
    return cache[n];
}

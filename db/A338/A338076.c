/*
 * A338076  Diagonal terms in the expansion of 1/(1-x-2*y-3*z).
 * https://oeis.org/A338076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^n * (3*n)! / (n!)^3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(n+1) = 18*(3*n+1)*(3*n+2)*a(n)/(n+1)^2 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr)
{
    size_t needed = (size_t)n + 1;
    if (*cap_ptr >= needed)
        return;
    size_t new_cap = (*cap_ptr > 0) ? (*cap_ptr * 2) : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    *cache_ptr = (seq_t *)realloc(*cache_ptr, new_cap * sizeof(seq_t));
    *cap_ptr = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cache_max  = 0;
    static bool   initialized = false;

    if (n == 0)
        return (seq_t)1;

    /* Ensure cache can hold index n */
    ensure_cache(n, &cache, &cache_cap);

    if (!initialized) {
        cache[0] = (seq_t)1;
        cache_max = 0;
        initialized = true;
    }

    if (cache_max >= n)
        return cache[n];

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        idx_t k = i - 1;   /* previous index                    */
        /* numerator = 18 * (3*k+1) * (3*k+2) */
        seq_t num = (seq_t)18;
        num = num * (seq_t)(3 * k + 1);
        num = num * (seq_t)(3 * k + 2);

        /* denominator = i * i */
        seq_t den = (seq_t)i;
        den = den * (seq_t)i;

        /* Cancel common factors between prev and den */
        seq_t prev = cache[i - 1];
        seq_t g = gcd(prev, den);
        prev = prev / g;
        den  = den  / g;

        /* Cancel common factors between num and remaining den */
        g = gcd(num, den);
        num = num / g;
        den = den / g;

        /* Now den divides prev * num exactly */
        cache[i] = prev * num / den;
    }

    cache_max = n;
    return cache[n];
}

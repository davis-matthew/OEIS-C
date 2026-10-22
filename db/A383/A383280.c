/*
 * A383280  a(n) = (3/2)^n * Sum_{k=0..n} (1/6)^k * (2*k)! * (n-k)! *
 *          binomial(n,k)^2.
 * https://oeis.org/A383280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ((n+1)*a(n-1) - (3*(n-1)^2/2)*a(n-2)), with a(0) = 1,
 *            a(1) = 2. Uses the three‑term recurrence to compute successive
 *            terms
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Utility: realloc with size_t; return NULL on failure (handled as no‑op). */
static void *safe_realloc(void *p, size_t old_cap, size_t new_cap, size_t elem_size)
{
    if (new_cap == 0) {
        free(p);
        return NULL;
    }
    return realloc(p, new_cap * elem_size);
}

seq_t a(idx_t n)
{
    static seq_t *cache    = NULL; /* array of already computed terms */
    static size_t capacity = 0;    /* array capacity in elements   */
    static size_t computed = 0;    /* number of terms stored (highest index+1) */

    size_t target = (size_t)n;     /* target index */

    /* Grow cache if necessary */
    if (target + 1 > capacity) {
        size_t new_cap = capacity > 0 ? capacity : 1;
        while (new_cap <= target)
            new_cap <<= 1;
        cache = (seq_t *)safe_realloc(cache, capacity, new_cap, sizeof(seq_t));
        capacity = new_cap;
    }

    /* Initialise few first terms if not already done */
    if (computed == 0) {
        cache[0] = 1;
        computed = 1;
    }
    if (computed == 1 && target >= 1) {
        cache[1] = 2;
        computed = 2;
    }

    /* Compute missing terms up to target */
    for (size_t i = computed; i <= target; ++i) {
        /* For n < 2 we already handled above */
        if (i < 2) {
            continue;
        }

        /* Use 128‑bit arithmetic for intermediate results */
        unsigned __int128 a_nm1 = (unsigned __int128)cache[i - 1];
        unsigned __int128 a_nm2 = (unsigned __int128)cache[i - 2];
        unsigned __int128 n_plus = (unsigned __int128)(i + 1);
        unsigned __int128 n_minus = (unsigned __int128)(i - 1);
        unsigned __int128 term1 = n_plus * a_nm1;            /* (n+1)*a(n-1)   */
        unsigned __int128 term2 = (unsigned __int128)3 * n_minus * n_minus * a_nm2; /* 3*(n-1)^2*a(n-2) */

        /* numerator: 2*term1 - term2 */
        unsigned __int128 diff = (unsigned __int128)2 * term1 - term2;

        /* a(i) = i * diff / 2 */
        unsigned __int128 numer = (unsigned __int128)i * diff;
        unsigned __int128 ai128 = numer / 2;

        cache[i] = (seq_t)ai128;   /* truncate if too large for seq_t */
    }

    computed = target + 1;
    return cache[target];
}

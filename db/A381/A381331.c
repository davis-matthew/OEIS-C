/*
 * A381331  a(1) = a(2) = 1; for n > 2, a(n) = floor((n - 2)*a(n - 1)/a(n -
 *          2)) + GCD(n - 2, a(n - 2)).
 * https://oeis.org/A381331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 115, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A381331
   Formula: a(1)=1; a(2)=1;
   for n>2: a(n)=floor((n-2)*a(n-1)/a(n-2))+gcd(n-2,a(n-2))
   (all arithmetic is integer arithmetic; gcd is the usual greatest common divisor). */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Static cache with 1‑based indexing: cache[0] == a(1) */
    static seq_t *cache = NULL;   /* array of already computed terms */
    static idx_t  cache_size = 0; /* allocated length of the array */
    static idx_t  computed   = 0; /* number of terms currently stored */

    /* Initialise cache with the two known terms. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return 0;                 /* unlikely failure */
        cache[0] = 1;                                 /* a(1) */
        cache[1] = 1;                                 /* a(2) */
        cache_size = 2;
        computed   = 2;
    }

    /* If the requested term is already known, return it. */
    if (n <= computed) return cache[n - 1];

    /* Grow the cache if necessary. */
    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (tmp == NULL) return 0;                  /* unlikely */
        cache      = tmp;
        cache_size = n;
    }

    /* Compute missing terms up to n inclusive. */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t a_k_1 = cache[k - 2]; /* a(k-1)  (index k-2 in 0‑based array) */
        seq_t a_k_2 = cache[k - 3]; /* a(k-2)  (index k-3)                 */

        /* floor((k-2)*a(k-1)/a(k-2))  */
        seq_t div;
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            div = (seq_t)(((__uint128_t)(k - 2) * a_k_1) / a_k_2);
        } else {
            div = (seq_t)(((seq_t)(k - 2)) * a_k_1 / a_k_2);
        }

        /* GCD term.  The helper gcd() works for uint*_t and for arbitrary‑precision types. */
        seq_t g = gcd((seq_t)(k - 2), a_k_2);

        /* Store the result. */
        cache[k - 1] = div + g;
    }

    computed = n;
    return cache[n - 1];
}

/*
 * A278024  Number of irreducible involutions of length n.
 * https://oeis.org/A278024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 19, u64 n <= 32, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Recurrence:
 * a(n) = -2*a(n-1) + (n-2)*a(n-2) + 3*(n-3)*a(n-3) + (4*n-13)*a(n-4)
 *        + 4*(n-5)*a(n-5) + 3*(n-6)*a(n-6) + (n-7)*a(n-7)
 * Initial values:
 * a(0)=1, a(1)=1, a(2)=1, a(3)=3, a(4)=5, a(5)=13, a(6)=37
 */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static int initialized = 0;

    /* Ensure cache capacity */
    if (cache == NULL) {
        cache_cap = (n > 7 ? n + 1 : 8);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failed */
    } else if (cache_cap <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failed */
        cache    = tmp;
        cache_cap = new_cap;
    }

    /* Initialize first seven terms only once */
    if (!initialized) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)13;
        cache[6] = (seq_t)37;
        initialized = 1;
    }

    if (n < 7) return cache[n];

    for (idx_t i = 7; i <= n; ++i) {
        seq_t term = 0;
        term += (seq_t)(i - 2)                * cache[i - 2];
        term += 3 * (seq_t)(i - 3)            * cache[i - 3];
        term += (seq_t)(4 * i - 13)           * cache[i - 4];
        term += 4 * (seq_t)(i - 5)            * cache[i - 5];
        term += 3 * (seq_t)(i - 6)            * cache[i - 6];
        term += (seq_t)(i - 7)                * cache[i - 7];
        term -= 2 * cache[i - 1];
        cache[i] = term;
    }

    return cache[n];
}

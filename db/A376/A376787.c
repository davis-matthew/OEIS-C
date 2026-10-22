/*
 * A376787  Expansion of (1 - x^2 + x^3)/((1 - x^2 + x^3)^2 - 4*x^3).
 * https://oeis.org/A376787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 80, u128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A376787
 * Recurrence: a(0)=1, a(1)=0, a(2)=1, a(3)=3, a(4)=1, a(5)=10
 *              a(n)=2*a(n-2)+2*a(n-3)-a(n-4)+2*a(n-5)-a(n-6) for n>=6
 * This code memoizes the values in a static array.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* memoization array */
static idx_t cached_cap = 0;  /* allocated capacity */
static idx_t cached_len = 0;  /* number of computed entries */

/* Ensure the cache can hold indices up to n */
static void
ensure_cache_cap(idx_t n) {
    if (n + 1 <= cached_cap)
        return;
    idx_t new_cap = cached_cap ? cached_cap * 2 : 8;
    while (new_cap <= n)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);
    cache = new_cache;
    cached_cap = new_cap;
}

/* Main function implementing the sequence */
seq_t a(idx_t n) {
    ensure_cache_cap(n);
    if (cached_len <= n) {
        /* Fill entries up to n */
        for (idx_t i = cached_len; i <= n; ++i) {
            switch (i) {
            case 0: cache[0] = (seq_t)1; break;
            case 1: cache[1] = (seq_t)0; break;
            case 2: cache[2] = (seq_t)1; break;
            case 3: cache[3] = (seq_t)3; break;
            case 4: cache[4] = (seq_t)1; break;
            case 5: cache[5] = (seq_t)10; break;
            default: {
                /* Compute using the recurrence:
                 * a(n)=2*a(n-2)+2*a(n-3)-a(n-4)+2*a(n-5)-a(n-6) */
                seq_t t = cache[i - 2];
                t = t + t;                 /* 2*a(i-2) */
                t = t + cache[i - 3];
                t = t + cache[i - 3];      /* +2*a(i-3) */
                t = t + cache[i - 5];
                t = t + cache[i - 5];      /* +2*a(i-5) */
                t = t - cache[i - 4];      /* -a(i-4) */
                t = t - cache[i - 6];      /* -a(i-6) */
                cache[i] = t;
            }
            }
        }
        cached_len = n + 1;
    }
    return cache[n];
}

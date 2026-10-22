/*
 * A336878  Second differences of A336877, divided by 3.
 * https://oeis.org/A336878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1, a(2)=1, a(3)=2. For n≥2: a(2*n) = a(n). For odd n≥3:
 *            a(n) = 3*a((n-1)/2) - a(n-2)
 * Range:     i8 n <= 94, i16 n <= 3070, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* External integer types provided by the build. */
extern void exit(int) __attribute__((noreturn));

seq_t a(idx_t n) {
    /* Static cache: 1‑based indexing. */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;

    if (n <= max_computed)
        return cache[n];

    /* Grow the cache to hold indices up to 'n'. */
    idx_t new_size = n + 1;               /* include index n */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        exit(1);                         /* out of memory */
    cache = tmp;

    /* Initialize base values if the cache was empty. */
    if (max_computed == 0) {
        cache[0] = 0;    /* unused slot for 1‑based indexing */
        cache[1] = 1;
        max_computed = 1;
    }
    if (max_computed < 2 && n >= 2) {
        cache[2] = 1;
        max_computed = 2;
    }

    /* Compute all new entries up to 'n'. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if ((i & 1) == 0) {               /* even index */
            cache[i] = cache[i >> 1];
        } else {                          /* odd index (i ≥ 3) */
            cache[i] = 3 * cache[(i - 1) >> 1] - cache[i - 2];
        }
    }
    max_computed = n;
    return cache[n];
}

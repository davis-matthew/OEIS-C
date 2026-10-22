/*
 * A345031  a(n) = 6*a(n-1) - 7*a(n-2) - 2*a(n-3) for n >= 3, with a(0) = a(1)
 *          = 0, a(2) = 1.
 * https://oeis.org/A345031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) - 2*a(n-3) (n ≥ 3) Initial values:
 *            a(0) = 0, a(1) = 0, a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;     /* array holding computed terms  */
    static size_t cache_len = 0;    /* current allocated length    */
    static size_t last_computed = (size_t)-1;  /* highest index computed */

    /* Handle the trivial base cases without allocating memory */
    if (n == 0 || n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Initialise or grow the cache so that it can hold up to n */
    if (cache == NULL) {
        size_t init = 3;                 /* room for a(0)…a(2) */
        if ((size_t)n + 1 > init) init = (size_t)n + 1;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (cache == NULL) exit(1);      /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache_len = init;
        last_computed = 2;
    } else if ((size_t)n + 1 > cache_len) {
        size_t new_len = cache_len * 2;
        while ((size_t)n + 1 > new_len) new_len *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
        cache_len = new_len;
    }

    /* Return the stored value if already computed */
    if ((size_t)n <= last_computed) return cache[(size_t)n];

    /* Compute new terms up to n using the recurrence */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        seq_t val = cache[i-1] * (seq_t)6;
        val -= cache[i-2] * (seq_t)7;
        val -= cache[i-3] * (seq_t)2;
        cache[i] = val;
    }
    last_computed = (size_t)n;
    return cache[(size_t)n];
}

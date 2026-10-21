/*
 * A247076  Number of tilings of a 5 X 2n rectangle using 2n pentominoes of
 *          shape P.
 * https://oeis.org/A247076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A247076 – Number of tilings of a 5×2n rectangle with 2n P‑pentominoes
 * Recurrence: a(n) = 2*a(n-1) + 2*a(n-2) + 5*a(n-3)  for n > 3
 * Initial terms: a(0)=1, a(1)=2, a(2)=6, a(3)=20
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;            /* array holding computed terms   */
    static idx_t cache_capacity = 0;       /* allocated storage size         */
    static idx_t cache_size     = 0;       /* number of terms currently filled */

    /* initialise cache with the first four values */
    if (cache_size == 0) {
        cache_capacity = 4;
        cache = (seq_t *)malloc((size_t)cache_capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* unlikely – signal failure      */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)20;
        cache_size = 4;
    }

    /* if requested term already computed, return it immediately */
    if (n < cache_size)
        return cache[n];

    /* ensure there is enough storage for indices up to n */
    if (n >= cache_capacity) {
        idx_t new_capacity = cache_capacity;
        if (new_capacity == 0) new_capacity = 4;
        while (new_capacity <= n)
            new_capacity <<= 1;               /* double until big enough      */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_capacity * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* unlikely – signal failure     */
        cache = tmp;
        cache_capacity = new_capacity;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t t1 = cache[i-1] * (seq_t)2;   /* a(i-1) * 2  */
        seq_t t2 = cache[i-2] * (seq_t)2;   /* a(i-2) * 2  */
        seq_t t3 = cache[i-3] * (seq_t)5;   /* a(i-3) * 5  */
        cache[i] = t1 + t2 + t3;
    }
    cache_size = n + 1;                     /* now have terms 0..n          */

    return cache[n];
}

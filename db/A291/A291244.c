/*
 * A291244  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - 4 S + S^2.
 * https://oeis.org/A291244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 4*a(n-3) - a(n-4) for n >= 5; *
 *            Initial values: a(0)=4, a(1)=15, a(2)=60, a(3)=239, a(4)=952
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cached = 0;       /* highest index computed so far */

    /* Initialise cache with the first five terms */
    if (cache == NULL) {
        size_t cap = 8;            /* start with a small capacity */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed – return 0 */

        cache[0] = (seq_t)4;
        cache[1] = (seq_t)15;
        cache[2] = (seq_t)60;
        cache[3] = (seq_t)239;
        cache[4] = (seq_t)952;
        cached   = 4;
    }

    /* Return value if already computed */
    if (n <= cached)
        return cache[n];

    /* Ensure the array is large enough */
    size_t cap = cached + 1;
    while (cap <= n) cap <<= 1;
    cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));

    /* Compute missing terms using the recurrence */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t t1 = cache[i-1] << 2;   /* 4 * a(i-1) */
        seq_t t2 = cache[i-3] << 2;   /* 4 * a(i-3) */
        cache[i] = t1 + cache[i-2] - t2 - cache[i-4];
    }
    cached = n;

    return cache[n];
}

/*
 * A261106  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00010001 00010101 or 01010101.
 * https://oeis.org/A261106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3), n > 6 with initial values:
 *            a(1)=28, a(2)=33, a(3)=63, a(4)=97, a(5)=183, a(6)=289
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 36, u64 n <= 76, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* cache[1..cap] holds computed terms */
    static idx_t cap = 0;         /* maximum n cached so far */

    if (n < 1) return (seq_t)0;   /* undefined for n<1, return 0 */

    /* initialise cache with the first six terms if needed */
    if (cache == NULL) {
        /* allocate space for indices 0..6 (ignore index 0) */
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        if (!cache) abort();
        cache[1] = (seq_t)28;
        cache[2] = (seq_t)33;
        cache[3] = (seq_t)63;
        cache[4] = (seq_t)97;
        cache[5] = (seq_t)183;
        cache[6] = (seq_t)289;
        cap = 6;
    }

    /* return cached value if already available */
    if (n <= cap) return cache[n];

    /* extend cache to hold new terms up to n */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute missing values using the recurrence */
    for (idx_t i = cap + 1; i <= n; ++i) {
        cache[i] = cache[i - 1]
                 + (seq_t)3 * cache[i - 2]
                 - (seq_t)3 * cache[i - 3];
    }
    cap = n;

    return cache[n];
}

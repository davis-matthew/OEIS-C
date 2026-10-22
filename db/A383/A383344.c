/*
 * A383344  Expansion of e.g.f. exp(-4*x) / (1-x)^4.
 * https://oeis.org/A383344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 1, 1 for n = 0; * a(n) = (n-1) * (a(n-1) + 4 *
 *            a(n-2)) for n > 1 * * The function memoizes computed values so
 *            that calls with arbitrary * order of arguments are efficient. It
 *            uses a dynamically resized * static array to store the sequence
 *            values. All arithmetic is performed * usi...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity (in elements)      */
    static idx_t last   = 1;   /* last index already computed           */

    /* initialise cache (size at least 2 for a[0] and a[1]) */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) { /* OOM – terminate execution */
            exit(1);
        }
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache[1] = (seq_t)0;   /* a(1) = 0 */
    }

    /* not necessary but guarantees correctness for very small n */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* grow cache when needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while ((size_t)n + 1 > new_cap) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* OOM – terminate execution */
            exit(1);
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t part1 = cache[i - 1];
        seq_t part2 = cache[i - 2];
        seq_t sum   = part1 + (seq_t)4 * part2;
        cache[i]    = ((seq_t)(i - 1)) * sum;
    }
    last = n;
    return cache[n];
}

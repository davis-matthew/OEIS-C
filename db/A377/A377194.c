/*
 * A377194  Expansion of 1/(1 - 4*x*(1+x)^2)^(3/2).
 * https://oeis.org/A377194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*(2*n+1)*a(n-1) + 8*(n+1)*a(n-2) + 2*(2*n+3)*a(n-3)) /
 *            n, a(0)=1, a(1)=6, a(2)=42
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* allocated capacity */
    static idx_t next = 0;       /* next index that needs computation */

    /* Allocate a small initial buffer if necessary */
    if (cache == NULL) {
        cache_size = 8;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0; /* memory allocation failed */
    }

    /* Initialise the first three known terms on first use */
    if (next == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)42;
        next = 3;
    }

    /* Grow the buffer until it can hold index n */
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (n >= new_size) new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0; /* realloc failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n */
    while (next <= n) {
        idx_t i = next;
        seq_t t1 = (seq_t)2 * (seq_t)(2 * i + 1) * cache[i - 1];
        seq_t t2 = (seq_t)8 * (seq_t)(i + 1) * cache[i - 2];
        seq_t t3 = (seq_t)2 * (seq_t)(2 * i + 3) * cache[i - 3];
        seq_t num = t1 + t2 + t3;
        cache[i] = num / (seq_t)i;
        ++next;
    }

    return cache[n];
}

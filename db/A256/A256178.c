/*
 * A256178  Expansion of exp( Sum_{n >= 1} L(2*n)*L(4*n)*x^n/n ), where L(n) =
 *          A000032(n) is a Lucas number.
 * https://oeis.org/A256178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 21, a(2) = 385, a(3) = 6930; a(n) = 21*a(n‑1) -
 *            56*a(n‑2) + 21*a(n‑3) - a(n‑4) (n ≥ 4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A256178.  */
seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;   /* cached terms */
    static idx_t  cache_len = 0;  /* number of elements stored */

    /* Initial construction of the cache with 4 terms */
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) abort();              /* memory allocation failed */

        cache[0] = (seq_t) 1;
        cache[1] = (seq_t) 21;
        cache[2] = (seq_t) 385;
        cache[3] = (seq_t) 6930;
    }

    /* Grow the cache up to n if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) abort();                 /* memory allocation failed */
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t term = (seq_t)21 * cache[i - 1]      /* 21*a(n-1) */
                         + (seq_t)21 * cache[i - 3];  /* +21*a(n-3) */
            term -= (seq_t)56 * cache[i - 2];          /* -56*a(n-2) */
            term -=          cache[i - 4];             /* - a(n-4) */
            cache[i] = term;
        }
        cache_len = new_len;
    }

    return cache[n];
}

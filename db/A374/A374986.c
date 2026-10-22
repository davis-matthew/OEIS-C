/*
 * A374986  a(n) = (1/5)*Product_{k=0..n} F(k)+5, where F=A000045 (Fibonacci
 *          numbers).
 * https://oeis.org/A374986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5)*Product_{k=0..n} (F(k)+5) (F = Fibonacci)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/fib.h"   /* provides seq_t fib(idx_t n) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[i] holds a(i)           */
    static idx_t cache_max = 0;      /* highest index computed so far */

    /* lazy initialization: compute a(0) = 1 */
    if (cache == NULL) {
        cache = (seq_t*)calloc(1, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* out of memory: return 0 */
        cache[0] = (seq_t)1;                 /* a(0) = 1 */
        cache_max = 0;
    }

    /* extend the cache if necessary */
    if (n > cache_max) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* out of memory */
        cache = tmp;

        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t f   = fib(i);          /* Fibonacci(i)    */
            seq_t fac = f + (seq_t)5;    /* (F(i)+5)        */
            cache[i] = cache[i-1] * fac; /* a(i) = a(i-1)*(F(i)+5) */
        }
        cache_max = n;
    }

    return cache[(size_t)n];
}

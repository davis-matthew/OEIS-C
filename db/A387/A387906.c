/*
 * A387906  a(n) = Sum_{k=0..n} binomial(4*n+3,5*k).
 * https://oeis.org/A387906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) + 130*a(n-2) + 740*a(n-3) - 65*a(n-4) + 16*a(n-5)  for n>4
   Initial terms: a(0)=1, a(1)=22, a(2)=474, a(3)=6008, a(4)=107883 */
 
#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache_size <= n) {
        /* Allocate or grow cache */
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        /* Compute terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)22;
            } else if (i == 2) {
                cache[i] = (seq_t)474;
            } else if (i == 3) {
                cache[i] = (seq_t)6008;
            } else if (i == 4) {
                cache[i] = (seq_t)107883;
            } else {
                seq_t term = (seq_t)5 * cache[i - 1];
                term += (seq_t)130 * cache[i - 2];
                term += (seq_t)740 * cache[i - 3];
                term -= (seq_t)65 * cache[i - 4];
                term += (seq_t)16 * cache[i - 5];
                cache[i] = term;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}

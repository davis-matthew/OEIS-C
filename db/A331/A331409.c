/*
 * A331409  a(1)=1; for n>1, a(n) = a(n-1)+n, divided by its largest prime
 *          factor.
 * https://oeis.org/A331409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n)= (a(n-1)+n) /
 *            largest_prime_factor(a(n-1)+n)
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[i] holds a(i+1) */
    static idx_t cached = 0;      /* number of terms stored in cache */
    if (n == 0) return 0;          /* undefined, but return 0 for safety */

    /* Ensure the cache covers up to n */
    if (cached < n) {
        /* allocate or resize cache */
        if (cached == 0) {
            cache = (seq_t *)malloc(n * sizeof(seq_t));
            if (!cache) return 0;           /* allocation failed */
            cached = 0;
        } else if (cached < n) {
            seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
            if (!tmp) return 0;           /* allocation failed */
            cache = tmp;
        }

        /* compute missing terms */
        for (idx_t i = cached + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {                 /* a(1) = 1 */
                val = 1;
            } else {
                uint64_t prev = (uint64_t)cache[i - 2];  /* a(i-1) */
                uint64_t sum  = prev + (uint64_t)i;       /* a(i-1)+i */
                uint64_t lpf  = largest_prime_factor(sum);/* largest prime factor */
                val = (seq_t)(sum / lpf);
            }
            cache[i - 1] = val;
        }
        cached = n;
    }
    return cache[n - 1];
}

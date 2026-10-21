/*
 * A251749  9-step Fibonacci sequence starting with 0,0,0,0,1,0,0,0,0.
 * https://oeis.org/A251749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 25, u32 n <= 41, u64 n <= 73, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 9‑step Fibonacci recurrence:
   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8)+a(n-9) for n ≥ 9
   a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=0, a(6)=0, a(7)=0, a(8)=0
*/
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial nine seed values */
    static const seq_t init[9] = {0, 0, 0, 0, 1, 0, 0, 0, 0};

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    size_t target = (size_t)n;
    if (cache_size <= target) {
        /* Allocate larger buffer */
        size_t new_size = target + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failed – return zero */

        /* Copy existing values */
        for (size_t i = 0; i < cache_size; ++i) {
            new_cache[i] = cache[i];
        }
        /* Release old memory */
        free(cache);

        /* Compute new values up to target */
        for (size_t i = cache_size; i <= target; ++i) {
            if (i < 9) {
                new_cache[i] = init[i];
            } else {
                seq_t sum = 0;
                for (size_t j = 1; j <= 9; ++j) {
                    sum += new_cache[i - j];
                }
                new_cache[i] = sum;
            }
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[target];
}

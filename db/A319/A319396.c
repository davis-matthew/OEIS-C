/*
 * A319396  Number of partitions of n into exactly three positive Fibonacci
 *          numbers.
 * https://oeis.org/A319396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8816, u16 n <= 8816, u32 n <= 8816, u64 n <= 8816,
 *            u128 n <= 8816, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A319396: Number of partitions of n into exactly three positive Fibonacci numbers.
   The function keeps a static cache of all values up to the greatest n requested
   and recomputes the cache whenever a larger n is queried. The Fibonacci numbers
   are stored in a dynamically allocated array of idx_t, avoiding any conversions
   between seq_t and idx_t.  All counts are held in the cache array of seq_t. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache     = NULL;   /* cache[i] = a(i) for 0 <= i < cache_cap */
    static size_t cache_cap = 0;      /* current capacity of cache array      */
    static idx_t *fibs      = NULL;   /* unique Fibonacci numbers <= current max n */
    static size_t fib_cnt   = 0;       /* number of Fibonacci numbers stored     */

    /* increase cache size and recompute values if n exceeds current capacity */
    if ((size_t)n >= cache_cap) {
        /* enlarge the cache to accommodate index n */
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;

        /* free previous Fibonacci list */
        if (fibs) { free(fibs); fibs = NULL; fib_cnt = 0; }

        /* generate Fibonacci numbers up to n (unique values: 1,2,3,5,8,...) */
        if (n >= 1) {
            size_t cap = 16;
            fibs = (idx_t*)malloc(cap * sizeof(idx_t));
            if (!fibs) return 0;       /* allocation failed */
            fibs[0] = 1;
            fib_cnt = 1;
            if (n >= 2) {
                if (fib_cnt == cap) {
                    cap <<= 1;
                    fibs = (idx_t*)realloc(fibs, cap * sizeof(idx_t));
                }
                fibs[1] = 2;
                fib_cnt = 2;
            }

            idx_t f0 = 1, f1 = 2;
            while (1) {
                idx_t next = f0 + f1;
                if (next > n) break;
                if (fib_cnt == cap) {
                    cap <<= 1;
                    fibs = (idx_t*)realloc(fibs, cap * sizeof(idx_t));
                }
                fibs[fib_cnt++] = next;
                f0 = f1;
                f1 = next;
            }
        }

        /* recompute all counts for 0 … n */
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        for (size_t i = 0; i < fib_cnt; ++i) {
            idx_t fi = fibs[i];
            for (size_t j = i; j < fib_cnt; ++j) {
                idx_t sum2 = fi + fibs[j];
                if (sum2 > n) break;
                for (size_t k = j; k < fib_cnt; ++k) {
                    idx_t sum = sum2 + fibs[k];
                    if (sum > n) break;
                    cache[sum]++;       /* increment count for this sum */
                }
            }
        }
    }

    return cache[(size_t)n];
}

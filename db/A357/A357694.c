/*
 * A357694  Number of ways to write n as an ordered sum of seven positive
 *          Fibonacci numbers (with a single type of 1).
 * https://oeis.org/A357694
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Form: a(n) = coefficient of x^n in (∑_{k≥2} x^{F(k)})^7
   where F(k) is the Fibonacci sequence starting 1,2,3,5,... (single 1)   */

#include <stdlib.h>

static seq_t *dp1 = NULL;          /* DP array for current number of parts */
static seq_t *dp2 = NULL;          /* Temporary DP array */
static seq_t *cache = NULL;        /* Final a(n) values, 0..max_n */
static idx_t dp_size = 0;          /* Allocated size of dp arrays */
static idx_t cache_size = 0;       /* Allocated size of cache array */

/* Build a(n) for all n <= limit, store in cache[0..limit] */
static void build_a(idx_t limit)
{
    idx_t size = limit + 1;
    if (dp1 == NULL || dp_size < size) {
        free(dp1);
        free(dp2);
        dp1 = (seq_t *)malloc(size * sizeof(seq_t));
        dp2 = (seq_t *)malloc(size * sizeof(seq_t));
        dp_size = size;
    }
    if (cache == NULL || cache_size < size) {
        free(cache);
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        cache_size = size;
    }

    /* zero dp1 */
    for (idx_t i = 0; i < size; ++i) dp1[i] = 0;

    /* dp1 for one part: one way for each Fibonacci number */
    if (limit >= 1) {
        idx_t fib = 1, fib_next = 2;
        while (fib <= limit) {
            dp1[fib] = 1;
            idx_t tmp = fib + fib_next;
            fib = fib_next;
            fib_next = tmp;
        }
    }

    /* Convolve for parts 2..7 */
    for (int part = 2; part <= 7; ++part) {
        /* zero temporary array */
        for (idx_t i = 0; i < size; ++i) dp2[i] = 0;

        /* add contributions from each Fibonacci value */
        idx_t fib = 1, fib_next = 2;
        while (fib <= limit) {
            for (idx_t s = fib; s <= limit; ++s) {
                dp2[s] += dp1[s - fib];
            }
            idx_t tmp = fib + fib_next;
            fib = fib_next;
            fib_next = tmp;
        }

        /* swap dp1 and dp2 for next iteration */
        seq_t *tmp_ptr = dp1;
        dp1 = dp2;
        dp2 = tmp_ptr;
    }

    /* copy final values into cache */
    for (idx_t i = 0; i < size; ++i) cache[i] = dp1[i];
}

/* Public function as required */
seq_t a(idx_t n)
{
    if (n < 7) return 0;                 /* undefined below offset, safe */
    if (n + 1 > cache_size)              /* need more data */
        build_a(n);
    return cache[n];
}

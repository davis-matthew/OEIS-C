/*
 * A331375  a(n) is the number of times the digit 1 appears in the
 *          concatenation of integers from 0 to n, minus the number of times
 *          the next most frequent digit appears.
 * https://oeis.org/A331375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = count_1(0..n) - max_{d≠1} count_d(0..n) (digit counts in
 *            concatenation of all integers from 0 to n)
 * Range:     i8 n <= 1111, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external dependencies other than <stdlib.h>.  The function keeps a
   cache of values so calls in any order are answered. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state: cache of computed a-values and current digit
       frequencies for numbers 0..current. */
    static seq_t *cache = NULL;      /* array of a(n) */
    static size_t cache_len = 0;     /* allocated size of cache */
    static seq_t freq[10];           /* total counts of digits 0–9 */
    static idx_t current = 0;        /* highest n whose a is cached */

    /* initialize on first call */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        /* digit counts for the initial concatenation "0" */
        freq[0] = (seq_t)1;
        freq[1] = freq[2] = freq[3] = freq[4] = freq[5] = 0;
        freq[6] = freq[7] = freq[8] = freq[9] = 0;
        /* a(0) = count_1 - max_{d≠1} count_d = 0 - 1 = -1 */
        cache[0] = (seq_t)-1;
        current = 0;
    }

    /* extend cache if n is larger than any previously cached value */
    if (n > current) {
        /* reallocate to hold indices up to n */
        cache_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_len);

        for (idx_t k = current + 1; k <= n; ++k) {
            /* count digits of k */
            idx_t x = k;
            while (x > 0) {
                unsigned d = (unsigned)(x % 10);
                ++freq[d];
                x /= 10;
            }

            /* compute a(k) */
            seq_t count1 = freq[1];
            seq_t max_other = freq[0];            /* start with digit 0 */
            for (unsigned d = 2; d <= 9; ++d) {
                if (d != 1 && freq[d] > max_other)
                    max_other = freq[d];
            }
            cache[k] = count1 - max_other;
        }

        current = n;
    }

    /* return cached result */
    return cache[(size_t)n];
}

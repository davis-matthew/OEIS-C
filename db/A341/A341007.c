/*
 * A341007  Numbers whose sum of even digits and sum of odd digits differ by
 *          6.
 * https://oeis.org/A341007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n-th positive integer whose sum of even digits and
 *            sum of odd digits differ by 6
 * Range:     u8 n <= 21, u16 n <= 3914, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* stored sequence values */
static size_t cache_size = 0;        /* allocated capacity */
static idx_t cached_terms = 0;       /* number of values computed */
static seq_t last_checked = 0;       /* last integer examined */

seq_t a(idx_t n)
{
    /* Return already-computed value if available. */
    if (n <= cached_terms)
        return cache[n - 1];

    /* Ensure the cache has room for new terms. */
    if (cache_size == 0) {
        cache_size = 256;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    }

    seq_t x = last_checked + 1;
    while (cached_terms < n) {
        seq_t y = x;
        seq_t sum_even = 0, sum_odd = 0;

        if (y == 0) {
            /* zero has even sum 0 and odd sum 0; skip it. */
        } else {
            while (y > 0) {
                seq_t digit = y % 10;
                if ((digit & 1) == 0)
                    sum_even += digit;
                else
                    sum_odd += digit;
                y /= 10;
            }
        }

        seq_t diff = (sum_even > sum_odd) ? sum_even - sum_odd : sum_odd - sum_even;
        if (diff == 6) {
            if (cached_terms == cache_size) {
                size_t new_size = cache_size * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
                cache = tmp;
                cache_size = new_size;
            }
            cache[cached_terms] = x;
            cached_terms++;
        }
        x++;
    }

    last_checked = x - 1;
    return cache[n - 1];
}

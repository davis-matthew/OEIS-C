/*
 * A381965  Irregular triangle read by rows, where row n lists the iterates of
 *          f(x), starting at x = n until f(x) < 10, where f(x) is the
 *          multiplicative digital root of x (A031347).
 * https://oeis.org/A381965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 709, u16 n <= 8315, u32 n <= 8315, u64 n <= 8315,
 *            u128 n <= 8315, big any n
 * Helpers:   digit_frequencies, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A381965: the “irregular triangle read by rows” defined
   by iterating the multiplicative digital root f(x) (A031347) starting
   at x = n until f(x) < 10.  The result a(n) is the n‑th element of the
   flattened triangle. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_frequencies.h"
#include "../utils/mul_small.h"

static seq_t *cache   = NULL;   /* cached triangular entries            */
static size_t cache_len = 0;     /* number of cached entries             */
static size_t cache_cap = 0;     /* allocated capacity of the cache      */
static idx_t  next_row  = 0;     /* row number whose first element is yet to be generated */

/* Return the product of the decimal digits of x.  Uses digit_frequencies
   to avoid extracting each digit individually. */
static seq_t product_of_digits(seq_t x)
{
    unsigned int freq[10] = {0};
    digit_frequencies(x, freq);

    /* any zero digit makes the product zero */
    if (freq[0]) return 0;

    seq_t prod = 1;
    for (int d = 1; d <= 9; ++d) {
        for (unsigned int i = 0; i < freq[d]; ++i)
            prod = mul_small(prod, (unsigned int)d);
    }
    return prod;
}

/* Return the n-th entry of the sequence.  The cache grows lazily on
   demand so that repeated calls are fast. */
seq_t a(idx_t n)
{
    while (cache_len <= (size_t)n) {                /* generate rows until we have n+1 items */
        /* Ensure there is space for at least one more entry */
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache     = tmp;
            cache_cap = new_cap;
        }

        /* Start a new row with the current row number as its first element */
        seq_t val = (seq_t)next_row;
        cache[cache_len++] = val;

        /* Generate the subsequent members of the row until the value is < 10 */
        while (val >= 10) {
            val = product_of_digits(val);

            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }

        ++next_row;   /* advance to the next row number */
    }

    return cache[(size_t)n];
}

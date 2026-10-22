/*
 * A358851  a(n+1) is the number of occurrences of the largest digit of a(n)
 *          among all the digits of [a(0), a(1), ..., a(n)], with a(0)=0.
 * https://oeis.org/A358851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 729, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0;
   For n ≥ 0 let m be the largest decimal digit of a(n).
   a(n+1) is the total number of occurrences of m among all decimal
   digits of the terms a(0), a(1), … , a(n).   */
/* Includes for symbols used below. */
#include <stddef.h>   /* NULL */
#include <stdlib.h>   /* realloc */

/* The utilities provide a way to count the decimal digits of a value. */
#include "../utils/digit_frequencies.h"

/* Global state for memoisation. */
static seq_t *cache = NULL;           /* computed a(n) values */
static idx_t  cache_len = 0;           /* number of values stored */
static unsigned int digit_count[10] = {0}; /* cumulative counts of digits 0..9 */

/* Return the largest decimal digit of v. */
static int largest_digit(seq_t v)
{
    unsigned int freq[10];
    digit_frequencies(v, freq);
    /* freq[d] > 0 → digit d occurs at least once.  Scan from 9 down. */
    int d;
    for (d = 9; d >= 0; --d)
        if (freq[d]) return d;
    return 0;   /* v is zero – only digit 0 occurs */
}

/* Add the digits of x to the global counter. */
static void add_digits_to_count(seq_t x)
{
    unsigned int freq[10];
    digit_frequencies(x, freq);
    unsigned int *cnt = digit_count;
    int d;
    for (d = 0; d < 10; ++d)
        cnt[d] += freq[d];
}

/* Memoised computation of the sequence.  Works for any call order. */
seq_t a(idx_t n)
{
    /* Expand cache if this is the first request or we need more values. */
    if (n >= cache_len) {
        /* Compute the new size for the cache. */
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));

        /* Initialise the first term on the very first expansion. */
        if (cache_len == 0) {
            cache[0] = 0;
            digit_count[0] = 1;   /* the single digit '0' of a(0) */
            cache_len = 1;
        }

        /* Compute every missing term up to index n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            int md = largest_digit(prev);        /* largest digit of a(i-1) */
            cache[i] = digit_count[md];          /* a(i) = count of md so far */
            add_digits_to_count(cache[i]);       /* make the new term available for later */
        }
        cache_len = new_len;
    }
    return cache[n];
}

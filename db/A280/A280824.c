/*
 * A280824  Numbers with an even number of digits and with an even number of
 *          distinct digits.
 * https://oeis.org/A280824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers with an even number of decimal digits and an even number
 *            of distinct digits. The function searches for the next candidate
 *            by incrementing a running value and testing the conditions using
 *            the provided digit utilities. Results are cached for efficient
 *            repeated queries in any order
 * Range:     u8 n <= 81, u16 n <= 5184, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"
#include "../utils/digit_len.h"

/* Static cache for previously found terms */
static seq_t *cache     = NULL;   /* array of found sequence values               */
static size_t cache_size = 0;      /* number of terms currently stored            */
static size_t cache_cap  = 0;      /* allocated number of elements                */
static seq_t next_val    = 0;      /* last candidate examined (next to test is next_val+1) */

/* Test whether a number satisfies the OEIS condition */
static bool valid(seq_t x)
{
    unsigned char cnt[10] = {0};
    /* digit_counts counts the occurrences of each decimal digit in x */
    digit_counts(x, cnt);
    int distinct = 0;
    for (int d = 0; d < 10; ++d)
        if (cnt[d]) ++distinct;

    int len = digit_len(x);          /* number of decimal digits in x   */
    return (len & 1) == 0 && (distinct & 1) == 0;
}

/* Ensure the cache contains at least n terms (n ≥ 1). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    /* Initialise capacity lazily */
    if (cache_cap == 0) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();                  /* allocation failure */
    }

    /* Search for further terms until we have n of them */
    while (cache_size < n) {
        ++next_val;                   /* move to next candidate          */
        if (valid(next_val)) {
            if (cache_size == cache_cap) {
                cache_cap <<= 1;           /* double capacity */
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    abort();
                }
                cache = tmp;
            }
            cache[cache_size++] = next_val;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined for n = 0, return 0 for safety */

    ensure_cache(n);
    return cache[n - 1];
}

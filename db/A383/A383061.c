/*
 * A383061  Positive integers that contain an odd digit d immediately preceded
 *          by a digit >= d.
 * https://oeis.org/A383061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_odd_preceded_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A383061.
 * Terms are the positive integers that contain an odd digit d
 * immediately preceded by a digit >= d.
 * The implementation discovers terms by brute‑force search
 * and caches them for subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_odd_preceded_digit.h"

/* Static cache data */
static seq_t *cache = NULL;   /* array of found terms */
static idx_t  cache_len = 0;  /* number of stored terms */
static idx_t  cache_cap = 0;  /* allocated capacity */

/* Current candidate being tested (static so all calls share). */
static seq_t next_candidate = 10;   /* start before first valid term */

/* Grow the cache to at least 'min_cap' elements. */
static void grow_cache(idx_t min_cap)
{
    if (cache_cap >= min_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* unrecoverable allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Required public function.  External linkage. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n<1, but return 0 */

    /* Generate terms until we have at least n entries */
    while (cache_len < n) {
        /* Search for the next valid integer */
        do {
            ++next_candidate;
        } while (!has_odd_preceded_digit(next_candidate));

        /* Ensure capacity and store the term */
        if (cache_cap < cache_len + 1) grow_cache(cache_len + 1);
        cache[cache_len++] = next_candidate;
    }

    return cache[n - 1];
}

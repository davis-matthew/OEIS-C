/*
 * A329296  Numbers whose digits are in nondecreasing order in bases 6 and 7.
 * https://oeis.org/A329296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   base7_nondecreasing
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: Numbers whose digits are in nondecreasing order
 * in bases 6 and 7 (OEIS A329296).  The implementation
 * generates all base‑6 non‑decreasing numbers length by length,
 * keeps the ones that satisfy the base‑7 property, and caches
 * them in an array so that a(n) can be returned for any n ≥ 1.
 * The function is thread‑unsafe but satisfies the required
 * performance for n up to about 10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/base7_nondecreasing.h"

/* --------- static cache ---------------------------------------------------- */
static seq_t *cache = NULL;          /* dynamic array of found numbers      */
static size_t cache_cap = 0;         /* allocated capacity                  */
static size_t cache_len = 0;         /* number of elements stored           */
static idx_t next_len = 1;           /* next base‑6 length to generate      */

/* Append a new term to the cache, growing the array if needed.  */
static void append_to_cache(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Recursive generator for all base‑6 numbers of length L
 * with non‑decreasing digits.  When a full length is reached,
 * the number is tested for the base‑7 property and, if it passes,
 * appended to the cache.  The accumulator 'acc' holds the value
 * built so far.  'prev' is the previous digit placed.  */
static void generate_len(idx_t L, idx_t pos, int prev, seq_t acc)
{
    if (pos > L) {                   /* full length obtained */
        if (base7_nondecreasing(acc))
            append_to_cache(acc);
        return;
    }

    int start = (pos == 1) ? 1 : prev;   /* leading digit cannot be 0 */
    for (int d = start; d <= 5; ++d) {
        seq_t next = acc * 6 + d;
        generate_len(L, pos + 1, d, next);
    }
}

/* Public sequence access function.  It lazily generates terms
 * until it has at least n of them, then returns the n‑th. */
seq_t a(idx_t n)
{
    /* Initialise cache with the first term (0). */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_len = 0;
        append_to_cache(0);          /* a(1) = 0 */
    }

    /* Generate terms until we have built up to a(n). */
    while (cache_len < n) {
        generate_len(next_len, 1, 0, 0);
        ++next_len;
    }

    return cache[n - 1];
}

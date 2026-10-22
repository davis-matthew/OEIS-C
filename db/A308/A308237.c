/*
 * A308237  Numbers m not ending with 0 that contain a digit, other than the
 *          leftmost digit, that can be removed such that the resulting number
 *          d divides m.
 * https://oeis.org/A308237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, changed
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 93, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   satisfies_condition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A308237 by brute‑force search up to 200000 and stores the
   results in a static cache.  The sequence is then returned by index
   (1‑based) from the cache. */

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "../utils/satisfies_condition.h"

static seq_t cache[200];
static size_t cached_len = 0;
static bool initialized = false;

/* Build the static cache once. */
static void build_cache(void)
{
    if (initialized) return;
    const unsigned long long UPPER = 200000ULL; /* covers the known maximum 180625 */
    for (unsigned long long m = 11; m <= UPPER; ++m) {
        if (satisfies_condition(m)) {
            if (cached_len < sizeof(cache)/sizeof(cache[0])) {
                cache[cached_len++] = (seq_t)m;
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined behaviour, return 0 for safety */
    build_cache();
    if ((idx_t)cached_len < n) return (seq_t)0;   /* out of bounds: safe fallback */
    return cache[n - 1];
}

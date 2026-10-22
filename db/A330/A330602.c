/*
 * A330602  a(n) = a(n-1) XOR (n+1), with a(0) = 0.
 * https://oeis.org/A330602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = a(n-1) XOR (n+1), a(0) = 0
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;

/* Ensure that cache[0..up_to] is computed */
static void ensure_cache(idx_t up_to)
{
    if (up_to < cache_len) return;

    /* Allocate space for the new entries */
    idx_t new_len = up_to + 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
    if (!tmp) return;          /* allocation failed – keep existing cache */
    cache = tmp;

    /* Compute all missing values */
    for (idx_t i = cache_len; i <= up_to; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
        } else {
            cache[i] = cache[i - 1] ^ (seq_t)(i + 1);
        }
    }
    cache_len = new_len;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

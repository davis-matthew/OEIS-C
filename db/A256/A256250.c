/*
 * A256250  Total number of ON states after n generations of a cellular
 *          automaton on the square grid.
 * https://oeis.org/A256250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = 1 + 4*A256249(n-1) with A256249(0) = 0 and for i >= 1
 *            A256249(i) = A256249(i-1) + 2*(i - h) + 1, where h = highest
 *            power of two <= i
 * Range:     u8 n <= 14, u16 n <= 232, u32 n <= 9538, u64 n <= 9538,
 *            u128 n <= 9538, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache for A256249 values */
static seq_t *cache_A249 = NULL;   /* cache_A249[i] == A256249(i) */
static size_t cache_len = 0;      /* number of entries that exist in the cache */

/* return highest power of two <= x (x >= 1) */
static idx_t highest_power_of_two_le(idx_t x)
{
    idx_t h = 1;
    while (h <= (x >> 1)) {
        h <<= 1;
    }
    return h;
}

/* grow the cache so that it contains values up to m (inclusive) */
static void ensure_cache_A249(idx_t m)
{
    if (m + 1 <= cache_len)   /* already computed */
        return;

    size_t new_len = (size_t)(m) + 1;
    seq_t *new_cache = (seq_t *)realloc(cache_A249, new_len * sizeof(seq_t));
    if (!new_cache)   /* allocation failure – leave the old cache intact */
        return;

    if (cache_len == 0) {    /* initialise the first element */
        new_cache[0] = (seq_t)0;   /* A256249(0) = 0 */
        cache_len = 1;
    }

    /* compute missing values */
    for (size_t i = cache_len; i <= (size_t)m; ++i) {
        idx_t idx = (idx_t)i;
        idx_t h = highest_power_of_two_le(idx);
        seq_t diff = (seq_t)2 * (seq_t)(idx - h) + (seq_t)1;
        new_cache[i] = new_cache[i - 1] + diff;
    }

    cache_A249 = new_cache;
    cache_len  = new_len;
}

/* Sequence A256250: total number of ON states after n generations. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* sequence is defined for n >= 1, but return 0 for safety */

    if (n == 1)
        return (seq_t)1;   /* a(1) = 1 + 4*A256249(0) = 1 */

    idx_t m = n - 1;            /* need A256249(m) */
    ensure_cache_A249(m);
    seq_t q = cache_A249[m];    /* A256249(n-1) */
    return (seq_t)1 + (seq_t)4 * q;
}

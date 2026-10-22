/*
 * A342650  Numbers divisible both by their nonzero individual digits and by
 *          the sum of their digits.
 * https://oeis.org/A342650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A simple search for the n‑th integer that is divisible by each
   of its nonzero digits and by the sum of its digits. */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

static bool is_good(seq_t v)
{
    if (v == 0) return true;

    seq_t sum = 0;
    seq_t temp = v;
    while (temp != 0) {
        seq_t d = temp % 10;
        sum += d;
        temp /= 10;
    }
    if (sum == 0) return false;
    if (v % sum != 0) return false;

    temp = v;
    while (temp != 0) {
        seq_t d = temp % 10;
        if (d != 0 && v % d != 0) return false;
        temp /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 1;

    /* Ensure sufficient cache space. */
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        if (is_good(next_candidate))
            cache[cache_len++] = next_candidate;
        next_candidate++;
    }

    return cache[(size_t)n - 1];
}

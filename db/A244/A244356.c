/*
 * A244356  Numbers n such that n and n+1 are not divisible by any of their
 *          nonzero digits.
 * https://oeis.org/A244356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 2528, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for OEIS sequence A244356 (Numbers n such that
 * n and n+1 are not divisible by any of their nonzero digits). */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
/* get_digits is provided by the utils library */
#include "../utils/get_digits.h"

/* true if n is divisible by at least one of its non‑zero decimal digits */
static bool divisible_by_any_digit(seq_t n)
{
    uint8_t buf[32];  /* enough for any reasonably sized integer */
    size_t len = 0;
    get_digits(n, buf, &len);
    for (size_t i = 0; i < len; ++i) {
        unsigned int d = buf[i];
        if (d == 0) continue;
        if (n % d == 0) return true;
    }
    return false;
}

/* sequence access -------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached sequence values          */
    static size_t cache_len = 0;    /* current number of cached terms  */
    static size_t cache_cap = 0;    /* allocated capacity of cache     */
    static seq_t cur = 1;           /* next integer to test            */

    if (n == 0)                      /* undefined offset: return 0     */
        return (seq_t)0;

    /* extend cache until it has at least n elements */
    while (cache_len < n) {
        /* find the next integer satisfying the property */
        while (true) {
            if (!divisible_by_any_digit(cur) &&
                !divisible_by_any_digit(cur + 1))
                break;
            ++cur;
        }

        /* add the found number to the cache */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 4 : cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();            /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = cur;
        ++cur;  /* move to the next candidate for the next search */
    }

    return cache[n - 1];
}

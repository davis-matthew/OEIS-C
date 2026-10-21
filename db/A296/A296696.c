/*
 * A296696  Numbers whose base-4 digits d(m), d(m-1), ... d(0) have #(rises) <
 *          #(falls); see Comments.
 * https://oeis.org/A296696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A296696: the n-th integer whose base‑4
 * digits have strictly fewer rises than falls.
 * The function searches successive integers and caches found terms
 * for future calls. */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether x satisfies rises < falls in its base‑4 expansion. */
static bool qualifies(seq_t x)
{
    /* base‑4 digits, least significant first */
    uint8_t d[64];          /* enough for any 64‑bit number, larger ones also fine */
    size_t len = 0;
    seq_t t = x;

    if (t == 0) return false;

    while (t != 0) {
        d[len++] = (uint8_t)(t % 4);
        t /= 4;
    }

    size_t rises = 0, falls = 0;
    for (size_t i = 1; i < len; ++i) {
        uint8_t high = d[i];      /* more significant digit */
        uint8_t low  = d[i - 1];  /* less significant digit */

        if (high > low) ++falls;
        else if (high < low) ++rises;
    }

    return rises < falls;
}

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache       = NULL;
    static size_t cache_cap   = 0;
    static idx_t cache_size   = 0;      /* number of cached terms */
    static seq_t last_checked = 0;      /* largest value examined so far */

    /* return cached value if present */
    if (n <= cache_size) {
        return cache[(size_t)(n - 1)];
    }

    /* initialise cache on first use */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);   /* allocation failure */
    }

    /* search for additional terms until we have the n-th one */
    for (seq_t x = last_checked + 1; ; ++x) {
        if (qualifies(x)) {
            /* grow cache if necessary */
            if ((size_t)cache_size >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                if (new_cap < cache_cap + 64) new_cap = cache_cap + 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[(size_t)cache_size++] = x;
            if (cache_size == n) {
                last_checked = x;
                return x;
            }
        }
    }
}

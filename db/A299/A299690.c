/*
 * A299690  Numbers without digit 1 whose multiplicative digital root is not
 *          0.
 * https://oeis.org/A299690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 4291, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A299690: numbers without digit 1 whose multiplicative
 * digital root is not 0.
 * Implementation: simple incremental search with a cache for
 * random‑access access in O(1) after O(n) first traversal.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/get_digits.h"
#include "../utils/digit_product.h"

static seq_t *cache    = NULL;  /* cached values of the sequence */
static size_t cache_len  = 0;   /* number of values stored */
static size_t cache_cap  = 0;   /* allocated capacity */
static idx_t  next_candidate = 0;   /* last number tested (0 means none yet) */
static bool   initialized = false;

/* Ensure cache is large enough to hold 'need' elements. */
static void
ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t newcap = cache_cap ? cache_cap * 2 : 8;
    if (newcap < need)
        newcap = need;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
    if (!tmp) abort();   /* allocation failed */
    cache = tmp;
    cache_cap = newcap;
}

/* a(n) implementation */
seq_t a(idx_t n)
{
    assert(n >= 1);

    if (!initialized) {
        /* initial allocation */
        cache_cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
        cache_len = 0;
        next_candidate = 0;
        initialized = true;
    }

    /* generate terms until we have at least n cached values */
    while (cache_len < n) {
        next_candidate++;               /* examine next integer */
        seq_t val = (seq_t)next_candidate;

        /* skip numbers containing the digit 1 */
        uint8_t digbuf[256];
        size_t dlen;
        get_digits(val, digbuf, &dlen);
        bool has_one = false;
        for (size_t i = 0; i < dlen; ++i) {
            if (digbuf[i] == 1) { has_one = true; break; }
        }
        if (has_one)
            continue;

        /* compute multiplicative digital root */
        seq_t prod = digit_product(val);
        while (prod >= 10)
            prod = digit_product(prod);

        if (prod == 0)   /* root is 0 → reject */
            continue;

        /* accept this value */
        ensure_capacity(cache_len + 1);
        cache[cache_len++] = val;
    }

    return cache[n - 1];
}

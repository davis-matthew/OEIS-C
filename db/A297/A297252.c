/*
 * A297252  Numbers whose base-4 digits have greater down-variation than
 *          up-variation; see Comments.
 * https://oeis.org/A297252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose base‑4 digits have greater down‑variation than up‑variation.
   For a given n ≥ 1 returns the nth such number. */

#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache      = NULL;  /* cache of found terms, 0‑indexed */
static size_t cache_len  = 0;     /* how many terms cached */
static size_t cache_cap  = 0;     /* allocated capacity */
static seq_t  last_num   = 0;     /* last natural number tested */

/* check whether the given number satisfies DV > UV in base‑4 */
static bool satisfies(seq_t n)
{
    uint8_t digits[64];   /* base‑4 digits, least significant first */
    size_t len = 0;
    seq_t tmp = n;
    do {
        digits[len++] = (uint8_t)(tmp % 4);
        tmp /= 4;
    } while (tmp != 0);

    seq_t dv = 0, uv = 0;
    for (size_t i = len; i-- > 1; ) {   /* iterate from most significant to least */
        uint8_t a = digits[i];
        uint8_t b = digits[i-1];
        if (a > b)
            dv += (seq_t)(a - b);
        else
            uv += (seq_t)(b - a);
    }
    return dv > uv;
}

/* append a value to the cache, expanding storage as needed */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* abort on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* external function required by the build */
seq_t a(idx_t n)
{
    /* a(1) is the first term. Return an error value (0) for n < 1. */
    if (n == 0) return 0;

    /* generate terms up to the requested index if necessary */
    while (cache_len < n) {
        seq_t cand = last_num + 1;
        if (satisfies(cand))
            cache_append(cand);
        last_num = cand;
    }
    return cache[n - 1];
}

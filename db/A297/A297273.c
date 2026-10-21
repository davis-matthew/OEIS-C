/*
 * A297273  Numbers whose base-11 digits have greater down-variation than
 *          up-variation; see Comments.
 * https://oeis.org/A297273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297273
 * Numbers whose base‑11 digits have greater down‑variation than up‑variation.
 * This implementation searches for successive numbers satisfying the
 * variation condition and memoises the found terms so that calls with arbitrary
 * indices are efficient. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static bool qualifies(seq_t x)
{
    /* Return true iff the base‑11 representation of x has
     * sum of decreases > sum of increases. */
    if (x < 2)   // single‑digit numbers cannot have variation
        return false;

    /* Collect base‑11 digits (little‑endian) */
    uint8_t buf[64];
    size_t len = 0;
    seq_t temp = x;
    while (temp > 0) {
        buf[len++] = (uint8_t)(temp % 11);
        temp /= 11;
    }

    seq_t dv = 0, uv = 0;          /* down‑variation, up‑variation */
    for (size_t i = len; i > 1; --i) {
        seq_t prev = buf[i - 1];
        seq_t curr = buf[i - 2];
        if (prev > curr)
            dv += (seq_t)(prev - curr);
        else if (curr > prev)
            uv += (seq_t)(curr - prev);
    }
    return dv > uv;
}

seq_t a(idx_t n)
{
    static seq_t *cache          = NULL;   /* memoised terms              */
    static idx_t  cache_cap     = 0;       /* allocated capacity           */
    static idx_t  cache_len     = 0;       /* number of stored terms       */
    static seq_t  cur            = 1;      /* next candidate number        */
    static bool   init           = false;  /* one‑time initialisation     */

    if (!init) {
        init = true;
        cache_len = 0;
        cache_cap = 0;
        cache = NULL;
        cur    = 1;
    }

    while (cache_len < n) {
        if (qualifies(cur)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }
        ++cur;
    }
    return cache[n - 1];
}

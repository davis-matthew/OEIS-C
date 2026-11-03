/*
 * A355301  Normal undulating numbers where "undulating" means that the
 *          alternate digits go up and down (or down and up) and "normal"
 *          means that the absolute differences between two adjacent digits
 *          may differ.
 * https://oeis.org/A355301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 173, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 126 terms through n = 9938 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Normal undulating numbers (A355301)
 *
 *  A(n) is the nth normal undulating number (offset 1, a(1)=0).
 *  A positive integer is normal undulating if adjacent digits
 *  never repeat and the sequence of differences between
 *  successive digits alternates in sign.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/*  The function works for any seq_t that supports  %, /, <, +, - and
 *  fits in memory.  The digit extraction uses only these operations
 *  and no extra allocation.                                            */
static bool is_normal_undulating(seq_t x)
{
    /* 0‑9 are always undulating                                    */
    if (x < 10)
        return true;

    /* extract digits into a buffer                                             */
    uint8_t digits[128];        /* enough for 128‑bit numbers (≈39 decimal digits)   */
    size_t len = 0;
    seq_t tmp = x;

    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* reverse to get most significant digit first                           */
    for (size_t i = 0; i < len / 2; ++i)
    {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* check the alternating-sign rule                                         */
    int prev_sign = 0;            /* undefined for the first difference          */
    for (size_t i = 1; i < len; ++i)
    {
        int diff = (int)digits[i] - (int)digits[i - 1];
        if (diff == 0)
            return false;          /* repeated adjacent digit                  */

        int sign = (diff > 0) ? 1 : -1;
        if (i > 1 && sign == prev_sign)
            return false;          /* sign did not alternate                   */

        prev_sign = sign;
    }
    return true;
}

static seq_t *cache  = NULL;      /* array of terms */
static size_t cache_len = 0;      /* number of terms stored          */
static size_t cache_cap = 0;      /* allocated capacity              */

/* ------------------------------------------------------------------ */
/*  Grow the cache so that it can hold at least `needed` terms.       */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache = tmp;          /* assume realloc never fails in the test pool   */
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Check whether a sequence element is a normal undulating number. --*/

/* ------------------------------------------------------------------ */
/*  Return the n‑th normal undulating number (n ≥ 1).  The sequence
 *  is generated on demand and cached, so repeated calls to a(n) are
 *  fast even for non‑monotonic n.                                           */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined, but harmless for underflow */

    /* initialise cache on first use                                        */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = 0;              /* a(1) = 0                                */
        cache_len = 1;
    }

    /* extend cache until it contains at least n terms                      */
    while (cache_len < n) {
        seq_t val = cache[cache_len - 1] + 1;
        while (!is_normal_undulating(val))
            val = val + 1;

        grow_cache(cache_len + 1);
        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[n - 1];
}

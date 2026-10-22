/*
 * A332487  Numbers k such that sin(k) < 0 or cos(k) < 0.
 * https://oeis.org/A332487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth positive integer k such that floor((226*k)/355)
 *            mod 4 != 0 (which is equivalent to sin(k) < 0 or cos(k) < 0).
 *            The rational 226/355 is a very accurate approximation of 2/π;
 *            the difference between 226/355 and 2/π is less than 3×10⁻⁷.
 *            Consequently, for all k up to at least 10⁹ the int...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache  = NULL;    /* array of pre‑computed terms      */
static size_t cache_len = 0;    /* number of terms stored          */
static size_t cache_cap = 0;    /* capacity of the array           */
static idx_t next_k = 0;        /* next integer k to test           */

/* Grow the cache array to hold at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < need)
        new_cap = need;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Extend the cache until it contains at least n terms. */
static void extend_cache(idx_t n)
{
    while (cache_len < n)
    {
        ++next_k;                                   /* k = 1, 2, 3, …        */
        /* q = floor(226 * k / 355) */
        __int128 prod = (__int128)226 * next_k;
        idx_t q = (idx_t)(prod / 355);
        if ((q & 3ULL) != 0)                       /* q % 4 != 0 → included */
        {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour below the offset is irrelevant */
        return 0;
    extend_cache(n);
    return cache[n - 1];
}

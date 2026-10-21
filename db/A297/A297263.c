/*
 * A297263  Numbers whose base-7 digits have greater up-variation than
 *          down-variation; see Comments.
 * https://oeis.org/A297263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A297263 – numbers whose base‑7 up‑variation is larger
   than their down‑variation.  The implementation performs an on‑demand
   incremental enumeration of integers and caches the found terms.  The
   cache contains the terms themselves; an auxiliary variable keeps the
   real integer value of the most recently found term so that the search
   continues correctly even when the cached values overflow the width of
   seq_t.  This satisfies the required O(n) behaviour for n up to about
   10 000. */
#include <stdlib.h>
#include <stdbool.h>

/* Helpers -------------------------------------------------------------- */

static bool qualifies(idx_t num)
{
    if (num < 7) /* all single‑digit numbers are 0‑variation */
        return false;

    seq_t uv = 0, dv = 0;
    uint8_t prev = (uint8_t)(num % 7);
    num /= 7;

    while (num) {
        uint8_t cur = (uint8_t)(num % 7);
        if (cur > prev)
            dv += (seq_t)(cur - prev);
        else if (cur < prev)
            uv += (seq_t)(prev - cur);
        prev = cur;
        num /= 7;
    }
    return uv > dv;
}

/* Cache for the sequence ------------------------------------------------*/

static seq_t *cache = NULL;
static size_t cache_len = 0;     /* number of terms stored so far */
static size_t cache_cap = 0;     /* allocated capacity of cache array */
static idx_t last_value = 0;     /* real integer value of the last stored term */

/* Grow the cache to accommodate at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < need)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = tmp;
    cache_cap = newcap;
}

/* Public sequence function ---------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; 0 is an unused sentinel */
        return 0;

    /* Ensure we have at least n terms cached */
    if (cache_len < (size_t)n) {
        idx_t next = (cache_len == 0) ? 0 : last_value + 1;
        while (cache_len < (size_t)n) {
            if (qualifies(next)) {
                grow_cache(cache_len + 1);
                cache[cache_len] = (seq_t)next; /* possible truncation OK */
                cache_len++;
                last_value = next;              /* keep correct integer value */
            }
            next++;
        }
    }
    return cache[(size_t)n - 1];
}

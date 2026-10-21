/*
 * A296130  Replace each digit of n with its complement to 9; this will
 *          reproduce all digits of n in a different order.
 * https://oeis.org/A296130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose multiset of decimal
 *            digits is invariant under digit complementation d → 9‑d (i.e.
 *            count(d)=count(9‑d) for d = 0..4)
 * Range:     u8 n <= 9, u16 n <= 252, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 10;   /* start scanning from 10 */

/* Test if x satisfies count(d)=count(9-d) for d=0..4 */
static int has_property(idx_t x)
{
    int cnt[10] = {0};
    idx_t t = x;
    if (t == 0) return 0;               /* 0 never satisfies */
    while (t > 0) {
        int d = t % 10;
        ++cnt[d];
        t /= 10;
    }
    for (int d = 0; d < 5; ++d) {
        if (cnt[d] != cnt[9 - d]) return 0;
    }
    return 1;
}

/* Ensure cache contains at least n entries */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;                 /* offset is 1 */
    while (cache_len < n) {
        /* Find next number with the property */
        while (1) {
            if (has_property(last_checked)) {
                /* Append to cache */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)last_checked;
                ++last_checked;
                break;
            }
            ++last_checked;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for n < 1, but not used */
    ensure_cache(n);
    return cache[n - 1];
}

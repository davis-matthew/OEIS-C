/*
 * A309107  A member of a family of generalizations of van Eck's sequence as
 *          defined below.
 * https://oeis.org/A309107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) computed as a(1) = a(2) = 0. For n ≥ 1, find the largest m
 *            < n-1 with a(m) = a(n). If such m exists, a(n+1) = n - m;
 *            otherwise a(n+1) = 0. (This is the k = 0, l = 2 generalisation
 *            of van Eck’s sequence.)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache: holds up to 'len' terms (indices 1..len) in cache[0..len-1] */
static seq_t *cache = NULL;
static idx_t  cap   = 0;
static idx_t  len   = 0;

/* ensure cache has room for at least 'need' terms */
static void ensure_capacity(idx_t need)
{
    if (cap < need) {
        idx_t new_cap = cap ? cap * 2 : 2;
        if (new_cap < need) new_cap = need;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }
}

/* initialise cache with first two terms 0,0 */
static void initialise_cache(void)
{
    if (cache == NULL) {
        ensure_capacity(2);
        cache[0] = 0;
        cache[1] = 0;
        len = 2;
    }
}

/* compute sequence term a(n) */
seq_t a(idx_t n)
{
    initialise_cache();            /* guarantee cache initialised */

    if (n <= len)                  /* already computed */
        return cache[n-1];

    /* compute missing terms up to n */
    for (idx_t idx = len + 1; idx <= n; ++idx) {
        seq_t val_prev = cache[idx-2];  /* a(idx-1) */

        /* find largest m ≤ idx-3 with a(m) == a(idx-1) */
        idx_t m = 0;
        for (idx_t j = idx-3; j > 0; --j) {
            if (cache[j-1] == val_prev) {
                m = j;
                break;
            }
        }

        seq_t next_val = (m != 0) ? (seq_t)((idx-1) - m) : 0;
        ensure_capacity(idx);
        cache[idx-1] = next_val;
    }

    len = n;   /* update known length */
    return cache[n-1];
}

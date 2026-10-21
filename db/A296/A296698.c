/*
 * A296698  Numbers whose base-5 digits d(m), d(m-1), ... d(0) have #(rises) >
 *          #(falls); see Comments.
 * https://oeis.org/A296698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n‑th natural number whose base‑5 digits
   contain more rises than falls. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache      = NULL;   /* stored qualifying numbers */
static size_t cache_cap  = 0;      /* allocated capacity of cache */
static size_t cache_len  = 0;      /* number of elements currently in cache */
static seq_t next_start  = 1;      /* next number to test */

/* Return 1 if x has more rises than falls in its base‑5 expansion,
   0 otherwise.  A rise = d(i) < d(i+1), a fall = d(i) > d(i+1). */
static int qualifies(seq_t x)
{
    if (x < 5) return 0;                /* single digit: 0 rises, 0 falls */

    seq_t prev   = x % 5;
    x           /= 5;
    int rises   = 0, falls = 0;

    while (x > 0) {
        seq_t cur = x % 5;              /* next more significant digit */
        if (cur < prev)  ++rises;
        else if (cur > prev) ++falls;
        prev = cur;
        x   /= 5;
    }
    return rises > falls;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* value for out‑of‑range indices */

    size_t target = (size_t)n;

    /* Keep building the cache until it contains at least target elements. */
    while (cache_len < target) {
        /* Skip non‑qualifying numbers. */
        while (!qualifies(next_start)) ++next_start;

        /* Grow the cache if necessary. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? (cache_cap << 1) : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* allocation failure – fall back to 0 */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the qualifying number. */
        cache[cache_len++] = next_start;
        ++next_start;  /* move past the value just stored */
    }
    return cache[target - 1];
}

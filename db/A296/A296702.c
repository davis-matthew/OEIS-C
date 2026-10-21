/*
 * A296702  Numbers whose base-6 digits d(m), d(m-1), ... d(0) have #(rises) <
 *          #(falls); see Comments.
 * https://oeis.org/A296702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A296702: numbers whose base‑6 digits have fewer rises than falls */
/* The sequence is computed by searching for qualifying integers and
   caching the results.  The cache is expanded on demand so calls may
   arrive in any order. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: test whether a 64‑bit integer has fewer rises than falls in base 6. */
static bool qualifies(uint64_t val)
{
    if (val == 0) return false;                     /* 0 has no comparisons */
    unsigned prev = val % 6;
    val /= 6;
    int rises = 0, falls = 0;
    while (val) {
        unsigned cur = val % 6;
        val /= 6;
        if (cur < prev) rises++;
        else if (cur > prev) falls++;
        prev = cur;
    }
    return rises < falls;
}

seq_t a(idx_t n)
{
    /* Cache of previously found terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    if (n == 0) return 0;          /* safety: offset is 1 */

    /* largest value that fits in seq_t */
    static const seq_t _seq_max_val = (seq_t)~(seq_t)0;
    static const uint64_t _seq_max_u64 = (uint64_t)_seq_max_val;

    if (n > len) {
        /* Ensure storage of at least n elements */
        if (n > cap) {
            size_t newcap = cap ? cap : 4;
            while (newcap < n) newcap <<= 1;
            seq_t *newarr = (seq_t *)malloc(newcap * sizeof(seq_t));
            if (!newarr) return 0;            /* allocation failed */
            if (cache) {
                for (size_t i = 0; i < len; ++i)
                    newarr[i] = cache[i];
            }
            free(cache);
            cache = newarr;
            cap   = newcap;
        }

        /* Search for the next qualifying numbers */
        uint64_t num = (len == 0) ? 1ULL : ((uint64_t)cache[len - 1] + 1ULL);

        while (len < n) {
            if (num > _seq_max_u64) break;    /* cannot store larger values */

            if (qualifies(num))
                cache[len++] = (seq_t)num;

            num++;
        }
    }

    return cache[n - 1];
}

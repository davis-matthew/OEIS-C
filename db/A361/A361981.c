/*
 * A361981  a(1) = 1; a(n) = Sum_{k=2..n} (-1)^k * k^2 * a(floor(n/k)).
 * https://oeis.org/A361981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = Sum_{k=2..n} (-1)^k * k^2 * a(floor(n/k)), a(1) = 1. This
 *            implementation precomputes values up to the largest n requested
 *            using a divisor‑sum grouping technique for O(n·√n) time and O(n)
 *            memory. The optional prefix array allows each inner loop to sum
 *            k^2·(-1)^k in O(1)
 * Range:     i8 n <= 7, i16 n <= 55, i32 n <= 2047, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers.  The surrounding test harness provides seq_t and
   idx_t types. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache of computed a(n) values: cache[1] is a(1) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocation size (number of slots) */
    static size_t cache_len = 0;   /* highest n that has been computed */

    /* Static prefix array for partial sums of (-1)^k * k^2 */
    static seq_t *prefix = NULL;
    static size_t pref_cap = 0;    /* allocation size for prefix */

    /* Grow the a(n) cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failed */
        /* initialise new entries to zero */
        memset((void *)(tmp + cache_cap), 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Grow the prefix array if necessary */
    if ((size_t)n >= pref_cap) {
        size_t new_cap = pref_cap ? pref_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(prefix, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failed */
        /* initialise new entries to zero; later filled in */
        memset((void *)(tmp + pref_cap), 0, (new_cap - pref_cap) * sizeof(seq_t));
        prefix = tmp;
        pref_cap = new_cap;
    }

    /* Compute missing prefix sums up to n */
    if (pref_cap > 0 && pref_cap > cache_len) {
        /* prefix[0] should be zero */
        if (pref_cap > 0 && prefix[0] == 0) /* first time */ ;
        else if (pref_cap == 0) prefix = (seq_t *)malloc(sizeof(seq_t) * 1);
    }
    if (pref_cap > 0 && pref_cap > cache_len) {
        /* Compute prefix from cache_len+1 up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t sign = (i % (idx_t)2 == 0) ? 1 : -1;
            seq_t sq   = (seq_t)i * (seq_t)i;
            prefix[i] = prefix[i - 1] + sign * sq;
        }
    }

    /* Compute a(n) for all indices up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        for (idx_t l = 2; l <= i; ) {
            idx_t m = i / l;           /* floor(i/l) */
            idx_t r = i / m;           /* last k with same floor */
            /* sum_{k=l..r} (-1)^k * k^2 = prefix[r] - prefix[l-1] */
            seq_t grp = prefix[r] - prefix[l - 1];
            sum += grp * cache[m];
            l = r + 1;
        }
        cache[i] = sum;
    }
    cache_len = (size_t)n;

    return cache[n];
}

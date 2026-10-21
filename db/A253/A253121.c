/*
 * A253121  Numbers k such that the hexagonal number H(k) is equal to the sum
 *          of the octagonal numbers O(m), O(m+1), O(m+2) and O(m+3) for some
 *          m.
 * https://oeis.org/A253121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3) (n ≥ 4) initial values:
 *            a(1)=18, a(2)=1730, a(3)=169498
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#define MAX_CACHE 10000   /* limit for which we store intermediate terms */

static seq_t cache[MAX_CACHE];   /* cache[i] holds a(i+1) for 0 ≤ i < MAX_CACHE */
static idx_t cache_len = 0;      /* number of terms currently stored */

/* compute a(n) for any n ≥ 1, using the recurrence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1, return 0 safely */

    /* if n is within the cached range, return it directly */
    if (n <= cache_len)
        return cache[n - 1];

    /* if n is within our cache capacity, fill the cache up to n */
    if (n <= MAX_CACHE) {
        /* initialise first terms if cache is empty */
        if (cache_len == 0) {
            cache[0] = (seq_t)18;
            cache[1] = (seq_t)1730;
            cache[2] = (seq_t)169498;
            cache_len = 3;
        }
        /* compute missing terms using the recurrence */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)        val = (seq_t)18;
            else if (i == 2)   val = (seq_t)1730;
            else if (i == 3)   val = (seq_t)169498;
            else {
                /* cache indices are zero‑based:
                   cache[i-2] = a(i-1), cache[i-3] = a(i-2), cache[i-4] = a(i-3) */
                val = (seq_t)99 * cache[i - 2]
                    - (seq_t)99 * cache[i - 3]
                    + cache[i - 4];
            }
            cache[i - 1] = val;
        }
        cache_len = n;
        return cache[n - 1];
    }

    /* for n > MAX_CACHE, compute on the fly with a sliding window */
    seq_t a1 = (seq_t)18;           /* a(i-3)   */
    seq_t a2 = (seq_t)1730;         /* a(i-2)   */
    seq_t a3 = (seq_t)169498;       /* a(i-1)   */
    seq_t an = a3;                  /* current a(i) */

    if (n <= 3) return a3;          /* already handled earlier, but keep safe */

    /* start from i = 4 */
    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)99 * a3 - (seq_t)99 * a2 + a1;
        /* shift window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}

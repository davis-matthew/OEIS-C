/*
 * A263537  Integers k such that A098531(k) is divisible by A000071(k+2).
 * https://oeis.org/A263537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A263537: a(n) = a(n-1)+a(n-3)-a(n-4) for n>6
 * with initial values a(1)=1, a(2)=2, a(3)=13, a(4)=25, a(5)=31, a(6)=43. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Guard against out‑of‑range index (offset starts at 1). */
    if (n == 0) return (seq_t)0;

    /* Pre‑computed initial terms.  Index 0 is unused. */
    static const seq_t init[7] = {
        0,                /* dummy for 0 */
        1,                /* a(1) */
        2,                /* a(2) */
        13,               /* a(3) */
        25,               /* a(4) */
        31,               /* a(5) */
        43                /* a(6) */
    };

    /* Persistent cache of previously computed values. */
    static seq_t *cache = NULL;   /* array, indexed from 0 to ... */
    static size_t cap     = 0;    /* allocated size */
    static idx_t  computed = 0;   /* highest n for which cache[n] is valid */

    /* Allocate or grow the cache if necessary.  We need space for indices 0..n. */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        if (newcap < 16) newcap = 16;          /* small default size */
        if (cache == NULL) {
            cache = (seq_t*)malloc(newcap * sizeof(seq_t));
            if (!cache) exit(EXIT_FAILURE);
        } else {
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
        }
        cap = newcap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = init[i];
        } else {
            /* Recurrence: a(i) = a(i-1) + a(i-3) - a(i-4). */
            cache[i] = cache[i-1] + cache[i-3] - cache[i-4];
        }
    }
    computed = n;

    return cache[n];
}

/*
 * A392355  a(n) = Sum_{k=0..floor(4*n/7)} binomial(k,4*n-7*k).
 * https://oeis.org/A392355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(4*n/7)} binomial(k,4*n-7*k) Recurrence:
 *            a(n) = 4*a(n-2) - 6*a(n-4) + 4*a(n-6) + a(n-7) - a(n-8) Initial
 *            values: a(0)=1,a(1)=0,a(2)=1,a(3)=0,a(4)=1,a(5)=0,a(6)=1,a(7)=1
 * Range:     u8 n <= 20, u16 n <= 35, u32 n <= 65, u64 n <= 125,
 *            u128 n <= 245, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for A392355 */
seq_t a(idx_t n) {
    /* cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed_max = (idx_t)-1;    /* index of highest computed term */

    /* initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap   = 8;   /* store first 8 terms explicitly */
        cache       = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)0;
        cache[6] = (seq_t)1;
        cache[7] = (seq_t)1;
        computed_max = 7;
    }

    /* already computed? */
    if (n <= computed_max) return cache[n];

    /* compute missing entries up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* enlarge cache if necessary */
        if (i >= cache_cap) {
            idx_t new_cap = cache_cap * 2;
            if (new_cap <= i) new_cap = i + 1;
            seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* recurrence is valid for i >= 8 */
        if (i < 8) { /* safety, should not happen */
            continue;
        }

        /* compute using the recurrence:
           a(i) = 4*a(i-2) - 6*a(i-4) + 4*a(i-6) + a(i-7) - a(i-8)
           To avoid unsigned underflow we add positive terms first
           and then subtract the negative ones. */
        seq_t pos = (4 * cache[i-2]) + (4 * cache[i-6]) + cache[i-7];
        seq_t neg = (6 * cache[i-4]) + cache[i-8];
        cache[i] = pos - neg;
    }

    computed_max = n;
    return cache[n];
}

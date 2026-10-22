/*
 * A392351  a(n) = Sum_{k=0..floor(3*n/14)} binomial(3*n-13*k,k).
 * https://oeis.org/A392351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-5) + a(n-14); Initial values: a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=3, a(6)=6, a(7)=9, a(8)=12,
 *            a(9)=15, a(10)=24, a(11)=42, a(12)=69, a(13)=105
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 53, u64 n <= 105,
 *            u128 n <= 208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static memoization table */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* allocated capacity      */
    static idx_t last = 13;        /* last index computed     */

    /* initial (hard‑coded) values up to index 13 */
    static const seq_t init[14] = {
        1, 1, 1, 1, 1,
        3, 6, 9,12,15,
        24,42,69,105
    };

    /* allocate cache and fill initial values on first call */
    if (!cache) {
        cap = 14;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (idx_t i = 0; i < 14; ++i)
            cache[i] = init[i];
    }

    /* grow cache if necessary */
    if (n + 1 > cap) {
        cap = n + 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* return immediately if value already known */
    if (n <= last)
        return cache[n];

    /* compute values up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val;
        if (i < 14)          /* safety: though unlikely due to last=13 */
            val = init[i];
        else
            val = cache[i - 1] + (seq_t)3 * cache[i - 5] + cache[i - 14];
        cache[i] = val;
    }
    last = n;
    return cache[n];
}

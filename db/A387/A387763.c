/*
 * A387763  a(n) = Sum_{k=0..floor(n/3)} 2^k * binomial(2*k,2*n-6*k).
 * https://oeis.org/A387763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2^k * binomial(2*k,2*n-6*k)
 *            Recurrence:
 *            a(0)=1,a(1)=0,a(2)=0,a(3)=2,a(4)=2,a(5)=0,a(6)=4,a(7)=24 a(n) =
 *            4*a(n-3)+4*a(n-4)-4*a(n-6)+8*a(n-7)-4*a(n-8) (n>=8)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 40, u64 n <= 77, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size */
    static idx_t last_computed = (idx_t)-1;   /* highest index computed */

    /* allocate or grow cache to accommodate n */
    if (cache_size <= n) {
        idx_t new_size = (cache_size == 0 ? 8 : n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed – return 0 */
        cache = tmp;
        cache_size = new_size;
    }

    /* initialise first 8 terms once */
    if (last_computed == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)0;
        cache[6] = (seq_t)4;
        cache[7] = (seq_t)24;
        last_computed = 7;
    }

    /* return value if already computed */
    if (n <= last_computed)
        return cache[n];

    /* compute subsequent terms up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 7) continue;   /* base values already set */
        seq_t sum1 = cache[i - 3] + cache[i - 4];
        sum1 += cache[i - 7];
        sum1 += cache[i - 7];   /* 2 * a(i-7) */
        seq_t sum2 = cache[i - 6] + cache[i - 8];
        sum1 -= sum2;           /* subtract terms */
        cache[i] = sum1 * (seq_t)4;
    }

    last_computed = n;
    return cache[n];
}

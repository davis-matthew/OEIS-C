/*
 * A241256  Number of 2 X n 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or zero plus the sum of the elements
 *          above it or one plus the sum of the elements diagonally to its
 *          northwest or zero plus the sum of the elements antidiagonally to
 *          its northeast, modulo 4.
 * https://oeis.org/A241256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 89, u128 n <= 178,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A241256:
   a(1)=2, a(2)=5, a(3)=4, a(4)=10, a(5)=12, a(6)=22,
   a(7)=41, a(8)=59, a(9)=120, a(10)=171, a(11)=306, a(12)=494;
   for n>12: a(n)=2*a(n-2)+2*a(n-3)-a(n-4)-a(n-5)-a(n-6)+a(n-7)+2*a(n-8)+a(n-10) */
#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;

    if (n == 0) return 0;            /* undefined for offset but harmless */

    ensure_cache(n, &cache, &cap);   /* cache[0..n] is available, maybe uninitialized */

    static const seq_t init[13] = {
        0,                 /* unused 0 index */
        2, 5, 4, 10, 12,   /* a(1)–a(5)   */
        22, 41, 59, 120,   /* a(6)–a(9)   */
        171, 306, 494      /* a(10)–a(12) */
    };

    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i]) continue;          /* already computed */

        if (i <= 12) {                   /* initial values */
            cache[i] = init[i];
            continue;
        }

        /* compute a(i) using the empirical recurrence */
        seq_t pos = ((seq_t)2 * cache[i-2]) +
                    ((seq_t)2 * cache[i-3]) +
                    cache[i-7] +
                    ((seq_t)2 * cache[i-8]) +
                    cache[i-10];
        seq_t neg = cache[i-4] + cache[i-5] + cache[i-6];
        cache[i] = pos - neg;
    }

    return cache[n];
}

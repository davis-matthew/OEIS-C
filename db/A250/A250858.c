/*
 * A250858  Number of (6+1) X (n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) (n > 7); Initial values: a(1)=233683,
 *            a(2)=1280447, a(3)=4648157, a(4)=13263136, a(5)=32201019,
 *            a(6)=69543783, a(7)=137379337
 * Range:     u8 none, u16 none, u32 n <= 14, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_n = 0;

    if (max_n == 0) {
        /* Ensure space for indices 0..7 */
        ensure_cache(8, &cache, &cap);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)233683;
        cache[2] = (seq_t)1280447;
        cache[3] = (seq_t)4648157;
        cache[4] = (seq_t)13263136;
        cache[5] = (seq_t)32201019;
        cache[6] = (seq_t)69543783;
        cache[7] = (seq_t)137379337;
        max_n = 7;
    }

    if (n <= max_n)
        return cache[n];

    /* Grow cache if necessary */
    ensure_cache((idx_t)(n + 1), &cache, &cap);

    for (idx_t i = max_n + 1; i <= n; ++i) {
        cache[i] = ( (seq_t)7  * cache[i-1] )
                 - ( (seq_t)21 * cache[i-2] )
                 + ( (seq_t)35 * cache[i-3] )
                 - ( (seq_t)35 * cache[i-4] )
                 + ( (seq_t)21 * cache[i-5] )
                 - ( (seq_t)7  * cache[i-6] )
                 + ( (seq_t)1  * cache[i-7] );
    }
    max_n = n;
    return cache[n];
}

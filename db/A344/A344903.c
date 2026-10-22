/*
 * A344903  a(n) is the number of optimal strategies for Player I in the
 *          Penney-Ante game with strings of length n.
 * https://oeis.org/A344903
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - (-1)^n * a(floor(n/2)+1) Initial values:
 *            a(3)=4, a(4)=2, a(5)=2
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;                      /* undefined part of the sequence */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    ensure_cache(n, &cache, &cache_cap);       /* allocate enough space, zeroed */

    if (cache[n] != 0) return cache[n];        /* memoized value */

    seq_t result;
    if (n == 3)          result = 4;
    else if (n == 4)     result = 2;
    else if (n == 5)     result = 2;
    else {
        seq_t prev = a(n - 1);
        idx_t half = (n >> 1) + 1;             /* floor(n/2)+1 */
        seq_t mid  = a(half);
        if ((n & 1) == 0)                     /* n even: (-1)^n = 1 */
            result = 2 * prev - mid;
        else                                   /* n odd: (-1)^n = -1 */
            result = 2 * prev + mid;
    }

    cache[n] = result;
    return result;
}

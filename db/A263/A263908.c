/*
 * A263908  Number of (2n+1) X (3+2) 0..1 arrays with each row and column
 *          modulo 3 equal to 1, read as a binary number with top and left
 *          being the most significant bits.
 * https://oeis.org/A263908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*2^(1+2*n) + 11^(1+2*n) + 31*13^n + 29*31^n) / 81
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used for computation (valid for n >= 1):
 *   a(1) = 33
 *   a(2) = 2399
 *   a(3) = 252097
 *   a(4) = 29452071
 *   a(n) = 169*a(n-1) - 6387*a(n-2) + 71671*a(n-3) - 195052*a(n-4)
 *
 * The recurrence is evaluated as
 *   a(n) = (169*a(n-1) + 71671*a(n-3)) -
 *          (6387*a(n-2) + 195052*a(n-4))
 * which guarantees no unsigned underflow because the first
 * parenthesised sum is strictly larger than the second.
 */

#include <stdlib.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0)
        return 0;   /* undefined offset, safety return */

    /* Expand cache if necessary */
    if (n > cache_size) {
        seq_t *new_cache = seq_realloc(cache, cache_size, n);
        if (!new_cache)
            return 0; /* allocation failed – unlikely in test harness */

        cache = new_cache;
        /* Compute values from cache_size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1)        cache[i - 1] = 33;
            else if (i == 2)   cache[i - 1] = 2399;
            else if (i == 3)   cache[i - 1] = 252097;
            else if (i == 4)   cache[i - 1] = 29452071;
            else {
                seq_t part1 = 169 * cache[i - 2] + 71671 * cache[i - 4];
                seq_t part2 = 6387 * cache[i - 3] + 195052 * cache[i - 5];
                cache[i - 1] = part1 - part2;
            }
        }
        cache_size = n;
    }

    return cache[n - 1];
}

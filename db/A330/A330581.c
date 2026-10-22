/*
 * A330581  a(0) = 2; thereafter a(n) = a(n - 1)^n + 1.
 * https://oeis.org/A330581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^n + 1, a(0) = 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = (idx_t)-1;

    /* Ensure sufficient storage for indices up to n */
    ensure_capacity(n + 1, &cap, (void **)&cache, sizeof(seq_t));

    /* Compute any missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)2;
        } else {
            seq_t prev = cache[i - 1];
            seq_t pow_val = big_pow(prev, (seq_t)i);
            cache[i] = pow_val + (seq_t)1;
        }
    }
    last = n;
    return cache[n];
}

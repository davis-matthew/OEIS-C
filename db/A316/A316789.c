/*
 * A316789  Number of same-tree-factorizations of n.
 * https://oeis.org/A316789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{n = x^y, y > 1} a(x)^y
 * Range:     u8 n <= 4095, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, ensure_cache, floor_log2_uint64, int_nth_root,
 *            power_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>               /* size_t */
#include <stdint.h>               /* uint64_t */
#include "../utils/ensure_cache.h"
#include "../utils/floor_log2_uint64.h"
#include "../utils/big_pow.h"
#include "../utils/int_nth_root.h"
#include "../utils/power_limit.h"

seq_t a(idx_t n)
{
    /* Memoization cache.  cache[1] corresponds to a(1). */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    ensure_cache(n, &cache, &cap);          /* guarantees cache[0..n] non‑null, zeroed */

    if (cache[n] != 0) return cache[n];     /* already computed */

    if (n == 1) {                           /* base case */
        cache[1] = 1;
        return 1;
    }

    seq_t sum = 0;
    idx_t max_exp = floor_log2_uint64(n);   /* y > 1, y ≤ log₂(n) */
    for (idx_t y = 2; y <= max_exp; ++y) {
        uint64_t r = int_nth_root((uint64_t)n, (uint64_t)y);
        if (r <= 1) continue;              /* r must be > 1 */

        /* Verify r^y == n using safe multiplication */
        uint64_t pp = power_limit(r, (uint64_t)y, (uint64_t)n);
        if (pp != (uint64_t)n) continue;   /* not an exact y‑th power */

        seq_t a_r = a(r);                  /* recursive value for a(r)   */
        seq_t pow_val = big_pow(a_r, (seq_t)y);
        sum += pow_val;
    }

    cache[n] = 1 + sum;                     /* a(n) = 1 + sum            */
    return cache[n];
}

/*
 * A303822  Multiplicative with a(p^k) = 3^a(k).
 * https://oeis.org/A303822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 1295,
 *            u128 n <= 4095, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A303822: a(n) multiplicative with a(p^k) = 3^a(k)
 *  a(1) = 1
 *  For n = ∏ p_i^{k_i}  (k_i ≥ 1)  we have
 *      a(n) = 3^( Σ a(k_i) )
 *  This function implements the above definition and memoizes
 *  small indices for efficiency.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow3.h"

#define MAX_CACHE 128            // cache for small indices only

/* local memoization tables */
static seq_t  _a_cache[MAX_CACHE];
static bool   _a_known[MAX_CACHE] = { false };

/* recursive helper to compute a(n) */
static seq_t _a_rec(idx_t n) {
    if (n == 1)                          /* a(1) = 1 */
        return 1;

    /* return cached value if available and within cache range */
    if (n < MAX_CACHE && _a_known[n])
        return _a_cache[n];

    idx_t tmp = n;
    seq_t sum = 0;

    /* factorize n, accumulating  Σ a(e) over prime exponents */
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            sum += _a_rec(e);             /* a(e) */
        }
    }
    /* remaining prime factor (if any) has exponent 1 -> a(1)=1 */
    if (tmp > 1)                          /* prime left */
        sum += 1;

    /* a(n) = 3^sum */
    seq_t result = pow3((idx_t)sum);

    /* memoize when n is in cacheable range */
    if (n < MAX_CACHE) {
        _a_cache[n]  = result;
        _a_known[n]  = true;
    }
    return result;
}

/* external linkage required by the build system */
seq_t a(idx_t n) {
    return _a_rec(n);
}

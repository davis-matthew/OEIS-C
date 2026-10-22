/*
 * A332411  If n = Product (p_j^k_j) then a(n) = Sum (n^(pi(p_j) - 1)), where
 *          pi = A000720.
 * https://oeis.org/A332411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n} n^(pi(p)-1) where p runs over the distinct
 *            primes of n (pi is the prime-counting function)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   compute_pi, factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/compute_pi.h"   /* compute_pi */
#include "../utils/pow_uint.h"    /* pow_uint */

seq_t a(idx_t n) {
    /* Handle the offset: a(1) = 0  (no prime factors) */
    if (n == 1)
        return (seq_t)0;

    /* Prime factorisation of n.  The arrays are large enough for n <= 10^12. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t count = 0;
    factor_u64((uint64_t)n, primes, exps, &count);

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < count; ++i) {
        uint64_t p = primes[i];
        seq_t pi = compute_pi(p);            /* pi(p), 64‑bit integer in seq_t form */
        idx_t exp = (idx_t)(pi - 1);          /* exponent fits in idx_t for p ≤ n ≤ 10^4 */
        seq_t term = pow_uint((uint64_t)n, exp);
        sum += term;
    }
    return sum;
}

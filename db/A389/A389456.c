/*
 * A389456  The number of divisors of n that are terms in A048103 (that are
 *          not divisible by p^p for any prime p).
 * https://oeis.org/A389456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} (min(p−1, e)+1) (multiplicative)
 * Range:     u8 n <= 9441, u16 n <= 9441, u32 n <= 9441, u64 n <= 9441,
 *            u128 n <= 9441, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;          /* empty product */

    /* factor n */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        /* min(p-1, e) + 1 */
        uint64_t min_val = (e < (p - 1)) ? e : (p - 1);
        seq_t contrib = (seq_t)(min_val + 1);
        result *= contrib;
    }
    return result;
}

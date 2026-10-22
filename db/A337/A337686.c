/*
 * A337686  a(n) is the least multiplier k such that n*k has twice as many
 *          divisors as n.
 * https://oeis.org/A337686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least k ≥ 2 such that τ(n·k) = 2 · τ(n), where τ(m)
 *            is the number of divisors of m. (τ is obtained via
 *            divisor_count_u64 from the utils.)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    /* τ(n) */
    seq_t dn = divisor_count_u64((uint64_t)n);
    /* For odd n, k = 2 is always minimal */
    if ((n & 1) == 1) {
        return (seq_t)2;
    }
    /* For even n, search k = 2,3,... until condition holds */
    for (idx_t k = 2; ; ++k) {
        uint64_t prod = (uint64_t)n * (uint64_t)k;
        seq_t dnk = divisor_count_u64(prod);
        if (dnk == 2 * dn) {
            return (seq_t)k;
        }
        /* The loop terminates because a solution always exists
           and k is guaranteed to be small (well below 10⁴ for n ≤ 10⁴). */
    }
}

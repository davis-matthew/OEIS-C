/*
 * A372917  a(n) is the number of distinct rectangles with area n whose
 *          vertices lie on points of a unit square grid.
 * https://oeis.org/A372917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct rectangles with area n whose vertices
 *            lie on points of a unit square grid. It is computed by summing,
 *            over all square‑free divisors r of n that are representable as a
 *            sum of two squares, the value ceil( d(n/r) / 2 ), where d(m) is
 *            the number of divisors of m. Since a squ...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* Factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Identify the primes that may appear in a square‑free r
       (i.e. primes p ≡ 1 (mod 4) and the prime 2).          */
    size_t allow_map[64] = {0};          /* bitmask for each prime */
    size_t allow_cnt   = 0;              /* number of allowed primes   */
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] % 4 != 3) {        /* p ≠ 3 (mod 4)            */
            allow_map[i] = 1u << allow_cnt;
            ++allow_cnt;
        }
    }

    seq_t result = 0;
    size_t total_masks = 1u << allow_cnt;

    for (size_t mask = 0; mask < total_masks; ++mask) {
        /* Compute τ(m) where m = n / r.  r consists of the primes
           selected in 'mask' (each with exponent 1).                     */
        seq_t tau = 1;
        for (size_t i = 0; i < cnt; ++i) {
            int f = (mask & allow_map[i]) ? 1 : 0;  /* f = 1 if prime i
                                                       is selected in r    */
            tau *= (seq_t)(exps[i] - f + 1);
        }
        /* Add ceil(τ/2) to the sum */
        result += (tau + 1) / 2;
    }

    return result;
}

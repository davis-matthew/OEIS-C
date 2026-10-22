/*
 * A391074  a(n) is the number of nonuniform divisors of n.
 * https://oeis.org/A391074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n minus the number of uniform
 *            divisors of n. A divisor d is uniform if, among the primes that
 *            actually divide d, all exponents are equal. (The divisor 1 is
 *            considered uniform.) This is implemented as: a(n) = τ(n) - (1 +
 *            ∑_{∅≠S⊆P(n)} min_{p∈S} e_p), where τ is the divi...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
/* Include the factorization helper provided in the build. */
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factor n.  The helper places distinct primes in primes[] and
       their exponents in exps[], with count in cnt. */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* τ(n)  = product (e_i + 1). */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (seq_t)(exps[i] + 1);

    /* Count uniform divisors.
       1 is always uniform. */
    seq_t uniform = 1;

    if (cnt > 0) {
        idx_t max_mask = ((idx_t)1 << cnt);
        for (idx_t mask = 1; mask < max_mask; ++mask) {
            /* Find the smallest exponent among the selected primes. */
            uint64_t min_e = (uint64_t)(-1);
            for (size_t i = 0; i < cnt; ++i) {
                if (mask & ((idx_t)1 << i)) {
                    if (exps[i] < min_e)
                        min_e = exps[i];
                }
            }
            uniform += (seq_t)min_e;
        }
    }

    return tau - uniform;
}

/*
 * A351656  Dirichlet g.f.: Product_{p prime} 1 / (1 - p^(-s) - p^(-2*s) -
 *          p^(-3*s) - p^(-4*s)).
 * https://oeis.org/A351656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 1023, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative sequence:  a(p^e) = f[e] where
   f[0] = 1, f[1] = 1, f[2] = 2, f[3] = 4 and
   f[e] = f[e-1] + f[e-2] + f[e-3] + f[e-4] for e >= 4.
   For a general n, a(n) = ∏_p f[e_p] where e_p is the exponent of prime p in n. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static seq_t f_values[65];
static bool f_initialised = false;

static void init_f_values(void) {
    if (f_initialised) return;
    f_values[0] = (seq_t)1;
    f_values[1] = (seq_t)1;
    f_values[2] = (seq_t)2;
    f_values[3] = (seq_t)4;
    for (int i = 4; i < 65; ++i) {
        f_values[i] = f_values[i-1] + f_values[i-2] + f_values[i-3] + f_values[i-4];
    }
    f_initialised = true;
}

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;    /* undefined below offset; harmless */

    init_f_values();

    /* Factor n using the utility factor_u64. */
    uint64_t primes[64];
    uint64_t exps [64];
    size_t  count = 0;
    factor_u64((uint64_t)n, primes, exps, &count);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < count; ++i) {
        /* exps[i] fits in the pre‑computed f_values table. */
        result *= f_values[(int)exps[i]];
    }
    return result;
}

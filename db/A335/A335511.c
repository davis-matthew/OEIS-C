/*
 * A335511  Number of (1,1,1)-avoiding permutations of the prime indices of n.
 * https://oeis.org/A335511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n has any prime exponent ≥ 3, otherwise a(n) = (Σ
 *            e_i)! / Π (e_i)! where n = ∏p_i^e_i is the prime factorisation
 *            of n (cubefree case). This is A008480(n) for cubefree n, 0
 *            otherwise
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   factor_u64, init_factorials
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include <stdbool.h>
#include "../utils/init_factorials.h"

seq_t a(idx_t n) {
    /* Static factorial table and one-time initialisation */
    static seq_t fact[65];
    static bool fact_inited = false;
    if (!fact_inited) {
        init_factorials(fact);
        fact_inited = true;
    }

    /* Factorise n; exponents fit in uint64_t and are usually small (<63) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Sum of exponents and check for cubefreedom */
    idx_t sum_exp = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] >= 3) return (seq_t)0;     /* n is not cubefree */
        sum_exp += (idx_t)exps[i];
    }

    /* Multinomial coefficient (Σ e_i)! / Π (e_i)! */
    seq_t result = fact[(idx_t)sum_exp];
    for (size_t i = 0; i < cnt; ++i) {
        result /= fact[(idx_t)exps[i]];
    }

    return result;
}

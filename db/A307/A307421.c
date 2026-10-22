/*
 * A307421  Dirichlet g.f.: zeta(s) * zeta(3*s) / zeta(2*s).
 * https://oeis.org/A307421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if every prime exponent in n is not ≡ 2 (mod 3); a(n) =
 *            0 otherwise. (All values are 0 or 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* a(1) is defined to be 1 (empty product). */
    if (n <= 1) return (seq_t)1;

    uint64_t tmp = (uint64_t)n;
    /* Maximum number of distinct prime factors of a 64‑bit integer is < 65. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* factor_u64 returns the list of prime factors and their exponents. */
    factor_u64(tmp, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] % 3 == 2) {
            return (seq_t)0;          /* rule: exponent ≡ 2 (mod 3) → 0 */
        }
    }
    return (seq_t)1;                  /* all exponents ok → 1 */
}

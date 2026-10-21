/*
 * A255647  Expansion of (phi(q) * phi(q^22) + phi(q^2) * phi(q^11)) / 2 in
 *          powers of q where phi() is a Ramanujan theta function.
 * https://oeis.org/A255647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255647: Expansion of (phi(q) * phi(q^22) + phi(q^2) * phi(q^11)) / 2
 * Multiplicative definition:
 *   a(0) = 1
 *   For n > 0, let n = ∏ p_i^e_i.
 *   For each prime power:
 *     if p_i == 2 or p_i == 11  → factor value = 1
 *     else if Kronecker(-22, p_i) = +1
 *          → factor value = e_i + 1
 *      else  (Kronecker(-22, p_i) = -1)
 *          → factor value = (1 + (-1)^e_i) / 2  (equals 1 when e_i even, 0 when e_i odd)
 *   Then a(n) = ∏ factor value.
 *
 * This implementation factors n and evaluates the multiplicative formula.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n) {
    /* Handle the offset n = 0 */
    if (n == 0) return (seq_t)1;

    /* n = 1 has no prime factors → product is 1 */
    if (n == 1) return (seq_t)1;

    /* factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t factor;

        /* Special primes 2 and 11 */
        if (p == 2 || p == 11) {
            factor = (seq_t)1;
        } else {
            /* Kronecker(-22, p) = (-1|p) * (22|p)   (p odd prime) */
            int s_minus1 = (p % 4 == 1) ? 1 : -1;               /* (-1|p) */
            int s_22     = jacobi_symbol(22, p);                /* (22|p) */
            int ks = s_minus1 * s_22;                          /* Kronecker symbol */

            if (ks == 1) {
                /* e+1, cast to seq_t */
                factor = (seq_t)(e + 1);
            } else { /* ks == -1 */
                /* (1+(-1)^e)/2 : 1 if e even, 0 if e odd */
                factor = ((e & 1ULL) == 0) ? (seq_t)1 : (seq_t)0;
            }
        }

        result *= factor;

        /* If product becomes zero, break early */
        if (result == (seq_t)0) break;
    }

    return result;
}

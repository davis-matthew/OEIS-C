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
 * Validated: standard, pass, 103 terms through n = 8265 (2026-11-01)
 * Helpers:   factor_u64
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
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

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

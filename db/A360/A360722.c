/*
 * A360722  a(n) is the sum of infinitary divisors of n that are powerful
 *          (A001694).
 * https://oeis.org/A360722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A360722: sum of infinitary divisors of n that are powerful.
 * Formula: a(n) is multiplicative with a(p^e) = f(p,e) for e even,
 *          a(p^e) = f(p,e) – p for e odd.
 *          f(p,e) = Π_{k ≥ 0, e_k = 1} (p^(2^k) + 1),
 *          where e_k is the k‑th binary digit of the exponent e.
 * This implementation factors n, computes the contribution of each
 * prime power, and multiplies them together. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr,
                                    uint64_t *ex, size_t *cnt) */
#include "../utils/pow_uint.h"    /* pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n) {
    /* n ≥ 1 (offset 1).  Return 1 for n == 1. */
    if (n == 0) return 0;           /* not used */
    uint64_t nn = (uint64_t)n;

    /* Space for prime factors (max ~64 for 64‑bit numbers). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt = 0;
    factor_u64(nn, primes, exps, &prime_cnt);

    seq_t result = 1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Compute f(p,e) = product over bits of e of (p^(2^k)+1). */
        seq_t f = 1;
        uint64_t exp_bits = e;
        int k = 0;
        while (exp_bits) {
            if (exp_bits & 1ULL) {
                /* p^(2^k) + 1 */
                seq_t term = pow_uint(p, ((idx_t)1) << k) + 1;
                f *= term;
            }
            exp_bits >>= 1;
            ++k;
        }

        /* Adjust for odd exponents. */
        if (e & 1ULL) {
            f -= (seq_t)p;
        }

        result *= f;
    }

    return result;
}

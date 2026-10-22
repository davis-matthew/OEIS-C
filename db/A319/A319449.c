/*
 * A319449  Sum of the norm of divisors of n over Eisenstein integers, with
 *          associated divisors counted only once.
 * https://oeis.org/A319449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 1492, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = product over prime powers p^e | n of local factors:
 *   • if p == 3:   a(3^e) = (3^(2e+1) - 1) / 2
 *   • if p ≡ 1 (mod 3): a(p^e) = ((p^(e+1) - 1)/(p-1))^2
 *   • if p ≡ 2 (mod 3): a(p^e) = (p^(2e+2) - 1)/(p^2 - 1)
 * The function is multiplicative, hence a(n) = Π a(p_i^e_i).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == 0) {   /* offset starts at 1, a(0) is undefined; return 0 */
        return 0;
    }

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t local;

        if (p == 3) {
            /* a(3^e) = (3^(2e+1) - 1) / 2 */
            idx_t exp = (idx_t)(2 * e + 1);
            seq_t pow_term = pow_uint(3, exp);
            local = (pow_term - (seq_t)1) / (seq_t)2;
        } else if ((p % 3) == 1) {
            /* a(p^e) = ((p^(e+1) - 1)/(p-1))^2 */
            idx_t exp = (idx_t)(e + 1);
            seq_t sigma = (pow_uint(p, exp) - (seq_t)1) / (seq_t)(p - 1);
            local = sigma * sigma;
        } else { /* p % 3 == 2 */
            /* a(p^e) = (p^(2e+2) - 1)/(p^2 - 1) */
            idx_t exp = (idx_t)(2 * e + 2);
            seq_t numerator   = pow_uint(p, exp) - (seq_t)1;
            seq_t denominator = pow_uint(p, 2)   - (seq_t)1;
            local = numerator / denominator;
        }

        result *= local;
    }

    return result;
}

/*
 * A392450  The number of exponential unitary (or e-unitary) divisors of n
 *          that are exponentially squarefree numbers (A209061).
 * https://oeis.org/A392450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} 2^{ω₀(e)} where ω₀(e) = #{prime factors of e
 *            with exponent 1} (i.e. the number of square‑free unitary
 *            divisors of e)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"

/* Count the square‑free unitary divisors of e: 2^{#primes in e with exponent 1}. */
static seq_t unitary_squarefree_count(uint64_t e)
{
    uint64_t cnt = 0;
    uint64_t t = e;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            unsigned exp = 0;
            while (t % p == 0) {
                t /= p;
                ++exp;
            }
            if (exp == 1) {
                ++cnt;
            }
        }
    }
    if (t > 1)            /* remaining prime factor has exponent 1 */
        ++cnt;
    return pow2((idx_t)cnt);
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        result = result * unitary_squarefree_count(exps[i]);
    }

    return result;
}

/*
 * A400450  The number of divisors d of n such that gcd(d, n/d) is prime.
 * https://oeis.org/A400450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = 2^(ω(n)-1) * Σ_p min(e_p-1, 2), where n = ∏ p^e_p and
 *            ω(n) is the number of distinct primes of n. This implements OEIS
 *            A400450
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/mul_small.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    /* Factor n; store primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* cnt = ω(n), number of distinct prime factors. */
    seq_t omega = (seq_t)cnt;

    /* Compute b(n) = Σ_p min(e_p-1, 2). */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        uint64_t val = 0;
        if (e > 1) {
            val = e - 1;
            if (val > 2) val = 2;
        }
        sum = sum + (seq_t)val;  /* Works for arbitrary-precision seq_t. */
    }

    if (sum == (seq_t)0) return (seq_t)0;  /* n is square‑free */

    /* Compute 2^(ω-1) using repeated multiplication by 2. */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 1; i < omega; ++i) {
        pow2 = mul_small(pow2, 2);
    }

    /* a(n) = pow2 * sum. */
    return seq_mul(pow2, sum);
}

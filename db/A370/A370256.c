/*
 * A370256  The number of ways in which n can be expressed as b^2 * c^3, with
 *          b and c >= 1.
 * https://oeis.org/A370256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = number of representations n = b^2 * c^3 with b,c ≥ 1 This
 *            is a multiplicative function: for n = ∏ p_i^{e_i}, a(n) = ∏
 *            f(e_i) where f(e) = |{(α,β) ∈ ℕ_0² : 2α + 3β = e }|. For a fixed
 *            exponent e, solutions exist only for e ≡ 0 or 1 (mod 2), and the
 *            count is f(e) = 1 + ⌊(⌊e/3⌋ - (e mod 2))/2...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 8623 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static inline seq_t count_local(uint64_t e)
{
    uint64_t m = e / 3;
    uint64_t parity = e & 1ULL;      /* e mod 2 */
    if (m < parity) return (seq_t)0; /* no solution */
    return (seq_t)((m - parity) / 2 + 1);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* out of OEIS range */
    if (n == 1) return (seq_t)1;     /* a(1)=1 by definition */

    /* Temporary buffers for factor_u64; exponents fit in uint64_t */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        result *= count_local(exps[i]);
    }
    return result;
}

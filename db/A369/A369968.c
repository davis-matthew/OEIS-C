/*
 * A369968  a(n) = 1 if n is not multiple of 5, but its arithmetic derivative
 *          is, otherwise 0.
 * https://oeis.org/A369968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is not a multiple of 5 and its arithmetic
 *            derivative is * a multiple of 5; 0 otherwise. * * Using d(n) = n
 *            * Σ_i e_i / p_i for n = ∏ p_i^{e_i} * For n not divisible by 5,
 *            d(n) ≡ (n mod 5) * Σ_i (e_i mod 5) * inv(p_i mod 5) (mod 5). *
 *            If this is 0 we return 1, else 0
 * Range:     u8 n <= 9820, u16 n <= 9820, u32 n <= 9820, u64 n <= 9820,
 *            u128 n <= 9820, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

seq_t a(idx_t n)
{
    /* n is unsigned; check for the offset value 0 */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* n multiple of 5 → answer 0 */
    if ((uint64_t)n % 5 == 0)
        return (seq_t)0;

    /* Factorise n. 64 is more than enough for 64‑bit integers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Pre‑computed inverses modulo 5 (0 unused). */
    static const uint8_t invmod5[5] = {0, 1, 3, 2, 4};

    uint32_t n_mod5 = (uint32_t)((uint64_t)n % 5);
    uint32_t sum_mod5 = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint32_t p_mod5 = (uint32_t)(primes[i] % 5);
        uint32_t invp = invmod5[p_mod5];               /* inverse of p mod 5 */
        uint32_t e_mod5 = (uint32_t)(exps[i] % 5);      /* e mod 5 */

        sum_mod5 += (invp * e_mod5) % 5;
        sum_mod5 %= 5;
    }

    uint32_t der_mod5 = (n_mod5 * sum_mod5) % 5;

    return (seq_t)((der_mod5 == 0) ? 1 : 0);
}

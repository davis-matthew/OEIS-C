/*
 * A319445  Number of Eisenstein integers in a reduced system modulo n.
 * https://oeis.org/A319445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative with * a(1) = 1 * a(3^e) = 2 * 3^(2*e-1)
 *            * a(p^e) = (p-1)^2 * p^(2e-2) if p ≡ 1 (mod 3) * a(p^e) =
 *            (p^2-1) * p^(2e-2) if p ≡ 2 (mod 3) * for any n ≥ 1
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

static inline seq_t local_contribution(uint64_t p, uint64_t e)
{
    if (p == 3u) {
        /* a(3^e) = 2 * 3^(2e-1) */
        seq_t pow3 = pow_uint(3u, (idx_t)(2*e - 1));
        return pow3 * ((seq_t)2);
    } else if (p % 3u == 1u) {
        /* a(p^e) = (p-1)^2 * p^(2e-2)  for p ≡ 1 (mod 3) */
        seq_t powp = pow_uint(p, (idx_t)(2*e - 2));
        seq_t t = (seq_t)(p - 1u);
        return powp * t * t;
    } else { /* p % 3 == 2 */
        /* a(p^e) = (p^2-1) * p^(2e-2)  for p ≡ 2 (mod 3) */
        seq_t powp = pow_uint(p, (idx_t)(2*e - 2));
        seq_t pp = (seq_t)p * (seq_t)p;
        return powp * (pp - (seq_t)1);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1u) return (seq_t)1;   /* a(1) = 1 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        result *= local_contribution(primes[i], exps[i]);
    }
    return result;
}

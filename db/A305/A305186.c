/*
 * A305186  Number of invertible 4 X 4 matrices mod n.
 * https://oeis.org/A305186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^16 * ∏_{p|n} (1 – 1/p)(1 – 1/p^2)(1 – 1/p^3)(1 – 1/p^4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 16, u128 n <= 256,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1, but return 0 for safety */
        return (seq_t)0;
    seq_t result = (seq_t)1;

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p    = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* (p‑1)(p²‑1)(p³‑1)(p⁴‑1) */
        seq_t term = p - (seq_t)1;
        seq_t p2   = p * p;
        term *= (p2 - (seq_t)1);
        seq_t p3   = p2 * p;
        term *= (p3 - (seq_t)1);
        seq_t p4   = p3 * p;
        term *= (p4 - (seq_t)1);

        /* p^(16e‑10) */
        seq_t pow_exp = (seq_t)(16ull * e - 10ull);   /* e ≥ 1 */
        seq_t pow_term = big_pow(p, pow_exp);

        term *= pow_term;
        result *= term;
    }

    return result;
}

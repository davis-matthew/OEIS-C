/*
 * A294705  Order of the general symplectic group of 4 X 4 matrices over Z_n.
 * https://oeis.org/A294705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} p^(11e-7)*(p-1)*(p^2-1)*(p^4-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 58, u128 n <= 3186,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Compute the order of the general symplectic group of 4×4 matrices over Z_n. */
seq_t a(idx_t n)
{
    /* n must be ≥ 1; if n == 0 return 0 (undefined, but safe). */
    if (n == 0) return 0;

    /* Factor n into primes and exponents. */
    /* A 64‑bit number has at most 64 prime factors (with multiplicity). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p  = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* p^(11*e-7) */
        seq_t p_pow = big_pow(p, (seq_t)(11*e - 7));

        /* (p-1) */
        seq_t term1 = p - (seq_t)1;

        /* (p^2-1) */
        seq_t p2    = p * p;
        seq_t term2 = p2 - (seq_t)1;

        /* (p^4-1) */
        seq_t p4    = p2 * p2;
        seq_t term3 = p4 - (seq_t)1;

        /* term = p_pow * term1 * term2 * term3 */
        seq_t term = p_pow;
        term = term * term1;
        term = term * term2;
        term = term * term3;

        result = result * term;
    }

    return result;
}

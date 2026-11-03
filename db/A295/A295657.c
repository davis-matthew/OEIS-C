/*
 * A295657  Multiplicative with a(p^e) = p^floor((e-1)/2).
 * https://oeis.org/A295657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9557 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A295657
   Formula: a(n) = Π_{p^e∥n} p^⌊(e−1)/2⌋  (product over prime powers in the factorisation of n) */

#include <stddef.h>          /* for size_t */
#include <stdint.h>          /* for uint64_t */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1) = 1, and we also treat 0 safely as 1 */
    if (n <= 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    /* Factor n into primes and exponents.
       factor_u64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
       fills `primes` and `exps` with the prime factors and their exponents,
       and stores the number of distinct primes in *cnt. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        /* t = floor((e-1)/2) */
        int t = (int)((exps[i] - 1) / 2);
        if (t <= 0)
            continue;

        seq_t p = (seq_t)primes[i];
        seq_t p_pow = (seq_t)1;

        for (int j = 0; j < t; ++j)
            p_pow *= p;

        result *= p_pow;
    }

    return result;
}

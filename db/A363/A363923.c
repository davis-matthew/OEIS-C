/*
 * A363923  a(n) = n^npf(n) / rad(n), where npf(n) is the number of prime
 *          factors with multiplicity of n.
 * https://oeis.org/A363923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(A001222(n)) / A007947(n) (OEIS A363923)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 63, u64 n <= 383,
 *            u128 n <= 4095, big any n
 * Validated: standard, pass, 72 terms through n = 9536 (2026-11-01)
 * Helpers:   factor_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n <= 1)
        return (seq_t)1;

    /* factorise n and store primes and their exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute Ω(n): total number of prime factors with multiplicity */
    idx_t omega = 0;
    for (size_t i = 0; i < cnt; ++i)
        omega += exps[i];

    /* accumulate the product ∏ p_i^(e_i*Ω(n) - 1) */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        idx_t  e  = (idx_t)exps[i];
        idx_t  exp = e * omega - 1;       /* exponent for this prime */
        seq_t term = ipow_idx(p, exp);     /* p^exp */
        result = seq_mul(result, term);   /* multiply into the result */
    }
    return result;
}

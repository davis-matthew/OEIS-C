/*
 * A356194  a(n) is the smallest multiple of n whose prime factorization
 *          exponents are all powers of 2.
 * https://oeis.org/A356194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} p^(2^⌈log₂(e)⌉) – smallest multiple of n whose
 *            prime‑factorization exponents are all powers of 2
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* next power of two ≥ e (e ≥ 1). */
static idx_t next_pow2(idx_t e)
{
    idx_t r = 1;
    while (r < e)
        r <<= 1;
    return r;
}

/* binary exponentiation: base^exp (exp ≥ 0). */
static seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)n;   /* a(1) = 1 */

    /* A 64‑bit integer has at most 16 distinct prime factors. */
    enum { MAX_FACTORS = 16 };
    uint64_t *pr = (uint64_t *)malloc(MAX_FACTORS * sizeof(uint64_t));
    uint64_t *ex = (uint64_t *)malloc(MAX_FACTORS * sizeof(uint64_t));
    if (!pr || !ex) {
        free(pr);
        free(ex);
        return (seq_t)0;   /* out‑of‑memory fallback – should never be hit */
    }

    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e   = (idx_t)ex[i];
        idx_t exp = next_pow2(e);          /* smallest 2^k ≥ e */
        seq_t p   = (seq_t)pr[i];
        result *= seq_pow(p, exp);         /* p^exp */
    }

    free(pr);
    free(ex);
    return result;
}

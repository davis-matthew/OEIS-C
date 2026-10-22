/*
 * A369718  The sum of unitary divisors of the smallest powerful number that
 *          is a multiple of n.
 * https://oeis.org/A369718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, look
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = product_{p | n} (1 + p^max(e_p,2))
 * where e_p is the exponent of prime p in the prime factorisation of n.
 * This is the sum of unitary divisors of the smallest powerful multiple of n.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* provides factor_u64() */

static seq_t pow_uint(uint64_t base, idx_t exp) {
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)base;
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n=0, but guard against bogus call */
    /* factor n: we need primes and their exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t   a  = (idx_t)exps[i];
        idx_t   e  = (a >= 2) ? a : 2u;       /* smallest powerful exponent */
        seq_t   term = pow_uint(p, e);        /* p^e */
        result *= (term + 1);                /* multiply by (1 + p^e) */
    }
    return result;
}

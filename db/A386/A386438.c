/*
 * A386438  a(n) = sigma(n) + omega(n) - n * Sum_{p|n, p prime} 1 / p.
 * https://oeis.org/A386438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) + omega(n) - n * Sum_{p|n} 1 / p
 * Range:     u8 n <= 16, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses helper functions provided by the harness:
   - void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
   - seq_t pow_uint(uint64_t base, idx_t exp)  */
/* See the OEIS definition of A386438. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Ensure n >= 1 (offset 1). For n == 0 we return 0, but it is irrelevant. */
    if (n == (idx_t)0)
        return (seq_t)0;

    uint64_t factors[64];
    uint64_t exponents[64];
    size_t cnt = 0;

    /* Factor n into primes only once per call. */
    factor_u64((uint64_t)n, factors, exponents, &cnt);

    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = factors[i];
        uint64_t e  = exponents[i];
        seq_t pow_val = pow_uint(p, e + 1);          /* p^(e+1) */
        seq_t term    = (pow_val - 1) / ((seq_t)p - 1); /* (p^(e+1)-1)/(p-1) */
        sigma *= term;
    }

    seq_t N         = (seq_t)n;    /* cast n to seq_t for arithmetic */
    seq_t sum_inv   = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t inv_term = N / (seq_t)factors[i];   /* n/p for each prime divisor */
        sum_inv += inv_term;
    }

    seq_t omega = (seq_t)cnt;
    return sigma + omega - sum_inv;
}

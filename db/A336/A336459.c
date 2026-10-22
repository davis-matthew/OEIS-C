/*
 * A336459  a(n) = A065330(sigma(sigma(n))), where A065330 is fully
 *          multiplicative with a(2) = a(3) = 1, and a(p) = p for primes p >
 *          3.
 * https://oeis.org/A336459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 448, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = A065330( sigma( sigma(n) ) )
   where A065330 is fully multiplicative with a(2)=a(3)=1 and
   a(p)=p for primes p>3.
   Algorithm: compute sigma(n) via prime factorisation, then
   sigma of that result (the numbers are small enough for uint64_t),
   then strip all factors 2 and 3 from the latter. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h" /* void factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */
#include "../utils/seq_pow.h"     /* seq_t seq_pow(seq_t, idx_t) */

/* Compute the divisor summatory function sigma for a 64‑bit integer. */
static seq_t sigma_uint64(uint64_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t pr[64];        /* prime factors */
    uint64_t ex[64];        /* exponents */
    size_t cnt;
    seq_t result = (seq_t)1;

    factor_u64(n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        idx_t e = (idx_t)ex[i];

        /* term = (p^(e+1) - 1) / (p - 1) */
        seq_t num   = seq_pow(p, (idx_t)(e + 1)) - (seq_t)1;
        seq_t den   = p - (seq_t)1;
        seq_t term  = num / den;

        result *= term;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* sigma(n)   */
    seq_t s = sigma_uint64((uint64_t)n);

    /* sigma(sigma(n))   */
    uint64_t s64 = (uint64_t)s;            /* fits in 64 bits for n ≤ 10000 */
    seq_t t = sigma_uint64(s64);

    /* strip factors 2 and 3 (A065330) */
    seq_t r = t;
    while (r % (seq_t)2 == 0) r /= (seq_t)2;
    while (r % (seq_t)3 == 0) r /= (seq_t)3;

    return r;
}

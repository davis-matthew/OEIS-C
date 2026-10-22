/*
 * A353237  a(n) = Sum_{d|n} (-1)^(d'), where d' is the arithmetic derivative
 *          of d (A003415).
 * https://oeis.org/A353237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^(d')
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _enum_divisors, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d' is the arithmetic derivative of the divisor d. */
/* This implementation enumerates all divisors by prime-factor exponents
   and computes the parity of d' directly, hence avoiding any large
   intermediate arithmetic. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/_enum_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    /* factor n: primes[], exps[], not more than 20 primes for 64‑bit n */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t result = 0;
    _enum_divisors(0, /* e0 = 0 */ 0, /* parity_other = 0 */ 0,
                   pcnt, exps, primes, &result);
    return result;
}

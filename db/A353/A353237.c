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
 * Validated: standard, pass, 102 terms through n = 9779 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d' is the arithmetic derivative of the divisor d. */
/* This implementation enumerates all divisors by prime-factor exponents
   and computes the parity of d' directly, hence avoiding any large
   intermediate arithmetic. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Recursive helper: enumerate all exponent combinations for the
   prime factorization of n.  For each divisor we compute its
   arithmetic-derivative parity as described in the analysis and
   accumulate the sign to *res. */
static void _enum_divisors(size_t idx, uint64_t e0, int parity_other,
                           size_t pcnt, const uint64_t exps[],
                           const uint64_t primes[], seq_t *res)
{
    if (idx == pcnt) {
        seq_t sign = 1;
        if (e0 == 1)          /* exactly one factor 2 -> odd derivative */
            sign = -1;
        else if (e0 == 0)     /* odd divisor: parity of sum of odd exponents */
            sign = (parity_other == 0) ? 1 : -1;
        /* else e0 >= 2 gives even derivative -> sign +1 */
        *res += sign;
        return;
    }

    const uint64_t prime   = primes[idx];
    const size_t   maxexp  = exps[idx];
    for (size_t exp = 0; exp <= maxexp; ++exp) {
        uint64_t new_e0   = e0;
        int      new_par  = parity_other;

        if (prime == 2) {
            new_e0 = exp;            /* prime 2 exponent updates e0 */
        } else {
            if (exp & 1U)            /* toggle parity of odd exponents */
                new_par ^= 1;
        }
        _enum_divisors(idx + 1, new_e0, new_par, pcnt, exps, primes, res);
    }
}

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

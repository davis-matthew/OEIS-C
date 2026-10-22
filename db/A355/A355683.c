/*
 * A355683  Multiplicative with a(p^e) = 0 if e=1 and a(p^e)= -1 if e>1.
 * https://oeis.org/A355683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(1) = 1; for n > 1, a(n) = 0 if n has a prime factor with
 *            exponent 1 (i.e., n is square‑free), otherwise a(n) =
 *            (-1)^{ω(n)} where ω(n) is the number of distinct prime factors
 *            of n. The function uses the supplied factor_u64() routine to
 *            factor n
 * Range:     i8 n <= 9169, i16 n <= 9169, i32 n <= 9169, i64 n <= 9169,
 *            i128 n <= 9169, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

#define MAX_FACTORS 64                /* sufficient for any 64‑bit integer */

static uint64_t primes[MAX_FACTORS];
static uint64_t exps  [MAX_FACTORS];

seq_t a(idx_t n)
{
    if (n == 1)        /* base case */
        return (seq_t)1;

    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* If any exponent equals 1, the product is 0. */
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] == 1)
            return (seq_t)0;

    /* All exponents are > 1: product of (-1)’s, one per distinct prime. */
    return (cnt & 1) ? (seq_t)-1 : (seq_t)1;
}

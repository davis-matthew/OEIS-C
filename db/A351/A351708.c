/*
 * A351708  a(n) = Sum_{p|n, p prime} (n/p)!.
 * https://oeis.org/A351708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{p|n, p prime} (n / p)!
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 25, u64 n <= 41, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 802 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* Computes A351708 for index n (offset 1). */
seq_t a(idx_t n)
{
    if (n < 2)          /* n = 1 has no prime divisors */
        return 0;

    /* Factor n into primes.  For n <= 10 000 the number of distinct primes
       cannot exceed 6; 64 slots is more than enough. */
    uint64_t primes[64];
    uint64_t exps[64];        /* exponents are unused */
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;

    for (size_t i = 0; i < cnt; ++i)
    {
        idx_t m = (idx_t)(n / primes[i]);  /* integer division, m = n/p */
        sum += factorial(m);
    }

    return sum;
}

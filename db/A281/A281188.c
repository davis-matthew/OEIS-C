/*
 * A281188  Number of refactorable numbers m such that tau(m) = n, or 0 if
 *          there are infinitely many such numbers.
 * https://oeis.org/A281188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 8706 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A281188: Number of refactorable numbers m with τ(m) = n,
 *          or 0 if infinitely many such m exist.
 * For n = 4: a(4) = 1 (only m = 8).
 * If n is not squarefree and n ≠ 4: a(n) = 0.
 * If n is squarefree with k distinct prime factors: a(n) = k!.
 *
 * The implementation factorises n to check squarefreeness,
 * counts the distinct primes, and returns the factorial.
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

seq_t a(idx_t n)
{
    /* Special case n = 4. */
    if (n == 4)
        return (seq_t)1;

    /* Factorise n.  Allocate arrays large enough for any n ≤ 10 000. */
    uint64_t x = (uint64_t)n;
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt;

    factor_u64(x, primes, exps, &cnt);

    /* If any exponent > 1, n is not squarefree → infinitely many solutions. */
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > 1)
            return (seq_t)0;
    }

    /* n is squarefree; a(n) = k! where k = number of distinct primes. */
    int k = (int)cnt;
    return factorial((idx_t)k);
}

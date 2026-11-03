/*
 * A368370  AGM transform of triangular numbers.
 * https://oeis.org/A368370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n+2,3)^n - n^n * n! * (n+1)! / 2^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-01)
 * Helpers:   binomial, ipow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include utility functions that provide the required arithmetic */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"
#include "../utils/pow2.h"

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

/* Computes the n-th term of OEIS sequence A368370. */
seq_t a(idx_t n)
{
    if (n < 1)                /* sequence is defined for n >= 1 */
        return (seq_t)0;

    /* C = binom(n+2,3)  ==  (n*(n+1)*(n+2))/6   */
    seq_t C = binomial(n + 2, 3);

    /* first term: C^n  */
    seq_t t1 = ipow(C, (seq_t)n);

    /* second term: n^n * n! * (n+1)! / 2^n   */
    seq_t t2 = ipow((seq_t)n, (seq_t)n);      /* n^n            */
    t2 = t2 * factorial(n);                     /* * n!           */
    t2 = t2 * factorial(n + 1);                 /* * (n+1)!       */
    t2 = t2 / pow2(n);                          /* / 2^n          */

    return t1 - t2;
}

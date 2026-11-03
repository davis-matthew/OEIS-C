/*
 * A345686  a(n) = n! * Sum_{k=1..n} n/floor(n/k)^2.
 * https://oeis.org/A345686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} n / floor(n/k)^2 (offset 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* local factorial: computes n! for the given seq_t type */
static seq_t factorial_local(idx_t n)
{
    seq_t prod = 1;
    for (idx_t i = 1; i <= n; ++i)
        prod = prod * (seq_t)i;
    return prod;
}

seq_t a(idx_t n)
{
    /* n must be >= 1 by the OEIS offset; behaviour for n == 0 is undefined */
    seq_t fact = factorial_local(n);        /* n!  */
    seq_t nseq = (seq_t)n;                  /* cast n to seq_t once  */
    seq_t multi = fact * nseq;              /* n! * n  */

    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t d = n / k;                    /* floor(n/k) */
        seq_t denom = (seq_t)d * (seq_t)d;   /* d^2   */
        seq_t term = multi / denom;         /* integer division: n!*n / d^2  */
        result += term;
    }
    return result;
}

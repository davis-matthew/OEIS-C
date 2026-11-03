/*
 * A249165  Number of cds-sortable permutations in S_n. That is, number of
 *          permutations for which application of some sequence of context
 *          directed swaps ("cds" operations) terminates in the identity.
 * https://oeis.org/A249165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   * a(n) = (n-1)! * (n+1)/2 if n is odd * a(n) = (n-1)! * (n^2 +
 *            3n - 2) / (2*(n+2)) if n is even * where (n-1)! is the factorial
 *            of (n-1). * * The function uses the helper factorial(idx_t) from
 *            the * utilities to compute the factorial in O(n). The rational *
 *            factor is applied safely using integer...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

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
    /* n >= 1 by problem definition */
    seq_t sn = (seq_t) n;          /* cast to seq_t for arithmetic */
    seq_t fac = factorial(n - 1);  /* (n-1)! */
    seq_t num, denom;

    if (n & 1ULL) {  /* odd n */
        num = sn + 1;
        denom = 2;
    } else {         /* even n */
        num = sn * sn + 3 * sn - 2;   /* n^2 + 3n - 2 */
        denom = 2 * (sn + 2);         /* 2*(n+2) */
    }

    return fac * num / denom;
}

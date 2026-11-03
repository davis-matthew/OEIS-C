/*
 * A364111  a(n) = Sum_{k = 0..n} binomial(n+k-1,k)^2 * binomial(2*n-2*k,n-k)
 *          * binomial(2*k,k).
 * https://oeis.org/A364111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k-1,k)^2 * binomial(2*n-2*k,n-k)
 *            * binomial(2*k,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A364111 as defined in the OEIS entry. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include <stddef.h>           /* for size_t, if needed */

/* External linkage. */
seq_t a(idx_t n)
{
    /* The term a(0) is given explicitly as 1. */
    if (n == 0)
        return 1;   /* seq_t literal */

    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(n+k-1, k) appears twice. */
        seq_t c = binomial(n + k - 1, k);
        seq_t term = c * c;         /* (binomial(n+k-1,k))^2 */
        term = term * binomial(2 * n - 2 * k, n - k);   /* * binomial(2*n-2*k, n-k) */
        term = term * binomial(2 * k, k);                 /* * binomial(2*k,k) */
        total += term;                     /* accumulate the sum */
    }
    return total;
}

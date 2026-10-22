/*
 * A356010  a(n) = n! * Sum_{k=1..n} floor(n/k)/k.
 * https://oeis.org/A356010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} floor(n/k)/k (OEIS A356010)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Compute a(n) for n >= 1, returns 0 for n==0 (undefined for OEIS offset 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset 1; a(0) is not defined */

    /* Allocate array to hold sigma values up to n.  Use calloc to zero-initialize. */
    seq_t *sigma = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!sigma)
        return (seq_t)0;          /* allocation failure – unlikely in tests */

    /* Sieve-like computation of sigma(k) = sum of divisors of k. */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t val = (seq_t)i;
        for (idx_t j = i; j <= n; j += i)
            sigma[j] += val;
    }

    /* Compute n! iteratively. */
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    /* Accumulate result: sum_{k=1..n} fact/k * sigma[k]  (exact integer division). */
    seq_t result = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = fact / (seq_t)k;      /* n! / k  – exact integer */
        term *= sigma[k];                  /* * sigma(k) */
        result += term;
    }

    free(sigma);
    return result;
}

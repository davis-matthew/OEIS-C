/*
 * A296194  Harary index of the n-transposition graph.
 * https://oeis.org/A296194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!/2) * Σ_{k=1}^{n-1} |Stirling1(n, n-k)| / k (unsigned
 *            Stirling numbers)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 93 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

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
    if (n < 1)   /* below offset, return 0 */
        return 0;

    /* Allocate array for Stirling numbers of the first kind (unsigned) up to order n. */
    seq_t *c = mem_new(seq_t, (size_t)(n + 1));
    if (!c)
        return 0;   /* allocation failed */

    /* Initialization: S(0,0)=1, S(0,k>0)=0 */
    for (idx_t i = 0; i <= n; ++i)
        c[i] = 0;
    c[0] = 1;

    /* Build successive rows.
       Recurrence: S(m,k) = S(m-1,k-1) + (m-1)*S(m-1,k)  (unsigned). */
    for (idx_t m = 1; m <= n; ++m) {
        for (idx_t k = m; k >= 1; --k) {
            c[k] = c[k - 1] + (seq_t)(m - 1) * c[k];
        }
        /* After the row, S(m,0)=0 */
        c[0] = 0;
    }

    /* Compute n! */
    seq_t fact = factorial(n);

    /* Sum the required terms: (n!/2) * Σ |S(n, n-k)| / k. */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n - 1; ++k) {
        seq_t denom = (seq_t)2 * (seq_t)k;
        seq_t term = fact * c[n - k];
        term = term / denom;          /* exact division */
        sum += term;
    }

    mem_free(c);
    return sum;
}

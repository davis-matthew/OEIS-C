/*
 * A363356  E.g.f. satisfies A(x) = exp(x * A(x) * (1 + x * A(x)^3)).
 * https://oeis.org/A363356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (3*n-2*k+1)^(k-1) * binomial(k,n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

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
    /* Handle the offset */
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* n! once for all terms */
    seq_t fact_n = factorial(n);
    seq_t res = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* binomial(k, n-k) is zero when k < n-k, giving no contribution */
        seq_t bin = binomial(k, n - k);
        if (bin == 0)
            continue;

        /* (3*n-2*k+1)^(k-1) */
        seq_t base = (seq_t)(3*n - 2*k + 1);
        seq_t pow  = ipow(base, (seq_t)(k - 1));

        /* ratio n! / k! (integer) */
        seq_t ratio = fact_n / factorial(k);

        /* term = ratio * bin * pow  */
        seq_t term = ratio * bin;
        term = term * pow;

        res += term;
    }
    return res;
}

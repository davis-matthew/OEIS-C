/*
 * A262709  Expansion of f(-x^4, -x^4) * f(-x, -x^5) in powers of x where f(,
 *          ) is Ramanujan's general theta function.
 * https://oeis.org/A262709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in q^(-1/3) * eta(q) * eta(q^4)^2 *
 *            eta(q^6)^2 / (eta(q^2) * eta(q^3) * eta(q^8)) where eta(q) =
 *            q^(1/24) ∏_{k≥1} (1 - q^k)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product can be written as a power series
   ∏_{m≥1} (1 - q^m)^{e_m} with
   e_m = 1 + 2[4|m] + 2[6|m] - [2|m] - [3|m] - [8|m].
   The coefficient a(n) is obtained by applying each factor
   sequentially to the series.  The following function
   computes a(n) in O(n^2) time, which is fast enough for
   n ≤ 10000. */

#include "../utils/seq_alloc_new_array.h"

seq_t a(idx_t n)
{
    /* Allocated array of coefficients up to n.  The array
       is recreated on every call; this keeps the implementation
       simple and works for all seq_t types supported by the
       build system.  For very large n the allocation size grows
       linearly. */
    seq_t *coeff = seq_alloc_new_array(n + 1);
    for (idx_t i = 0; i <= n; ++i)
        coeff[i] = 0;
    coeff[0] = 1;                    /* constant term */

    /* Process each factor (1 - q^m)^{e_m}.  For a positive
       exponent e, we multiply by the factor e times, using
       the recurrence c[k] := c[k] - c[k-m] (descending loop).
       For a negative exponent –e, we divide by the factor
       e times, using the recurrence c[k] := c[k] + c[k-m]
       (ascending loop). */
    for (idx_t m = 1; m <= n; ++m) {
        int e = 1;
        if (m % 4 == 0) e += 2;
        if (m % 6 == 0) e += 2;
        if (m % 2 == 0) e -= 1;
        if (m % 3 == 0) e -= 1;
        if (m % 8 == 0) e -= 1;
        if (e == 0) continue;

        if (e > 0) {
            for (int t = 0; t < e; ++t)
                for (int k = (int)n; k >= (int)m; --k)
                    coeff[k] -= coeff[k - m];
        } else { /* e < 0 */
            for (int t = 0; t < -e; ++t)
                for (idx_t k = m; k <= n; ++k)
                    coeff[k] += coeff[k - m];
        }
    }

    seq_t result = coeff[n];
    /* In the C++_build the array elements are automatically
       destroyed when coeff goes out of scope; in the C
       compilations the function returns a value, so we free
       the allocated memory to avoid a leak. */
    free(coeff);
    return result;
}

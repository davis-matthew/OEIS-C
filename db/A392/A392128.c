/*
 * A392128  E.g.f. A(x) satisfies A(x) = (1 + 5*x*A(x)^3)^(1/5).
 * https://oeis.org/A392128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ∏_{k=1}^{n-1} (3*n+1 - 5*k) (empty product = 1)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
/* If the build provides a helper you may include it; it is not required here. */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    seq_t base = (seq_t)3 * (seq_t)n + 1;
    for (idx_t k = 1; k < n; ++k) {
        seq_t term = base - (seq_t)5 * (seq_t)k;
        prod *= term;
    }
    return prod;
}

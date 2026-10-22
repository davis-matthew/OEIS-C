/*
 * A384167  a(n) = 2^n * n! * binomial(3*n/2,n) * Sum_{k=1..n} 1/(n+2*k).
 * https://oeis.org/A384167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A384167
   Formula: a(n) = ∏_{i=1}^{n} (n+2i)  *  Σ_{i=1}^{n} 1/(n+2i)
           = Σ_{i=1}^{n} ∏_{j≠i} (n+2j)
   (Offset: 1 – a(1) = 1)
   The value may be huge; seq_t is an arbitrary-precision or machine-sized integer.
   The algorithm computes the full product once and then sums the products
   with each factor removed.  All operations are performed using seq_t operators
   (+, -, *, /, %). No floating-point or external utilities are required.
*/
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;             // offset is 1; value for n=0 is not used

    seq_t prod = 1;                    // product of all (n+2*i)
    idx_t i;
    for (i = 1; i <= n; ++i) {
        prod *= (seq_t)(n + 2 * i);
    }

    seq_t sum = 0;                     // Σ prod / (n+2*i)
    for (i = 1; i <= n; ++i) {
        seq_t denom = (seq_t)(n + 2 * i);
        sum += prod / denom;
    }
    return sum;
}

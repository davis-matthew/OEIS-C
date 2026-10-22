/*
 * A362786  Number of unordered triples of disjoint self-avoiding paths with
 *          nodes that cover all vertices of a convex n-gon; one node paths
 *          are not allowed.
 * https://oeis.org/A362786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-12)*n*(n-1)*(n-2)*(n-4)*(n-5)*(n+2)*(n+9)/90 (for n
 *            != 3)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 20, u64 n <= 44, u128 n <= 100,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n <= 5 the product contains a zero factor or the division by
       a large power of two yields 0 */
    if (n <= 5) return 0;

    /* Compute the numerator product */
    seq_t prod = (seq_t)n;
    prod *= (seq_t)(n - 1);
    prod *= (seq_t)(n - 2);
    prod *= (seq_t)(n - 4);
    prod *= (seq_t)(n - 5);
    prod *= (seq_t)(n + 2);
    prod *= (seq_t)(n + 9);

    /* Divide by 90 */
    prod /= (seq_t)90;

    /* Apply the factor 2^(n-12) – multiply if n>=12, otherwise divide */
    if (n >= 12) {
        seq_t p = pow2(n - 12);
        prod *= p;
    } else {
        seq_t p = pow2(12 - n);
        prod /= p;
    }

    return prod;
}

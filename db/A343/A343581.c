/*
 * A343581  a(n) = binomial(n, floor(n/2))*FallingFactorial(n - 1, n -
 *          floor(n/2)).
 * https://oeis.org/A343581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, floor(n/2)) * FallingFactorial(n - 1, n -
 *            floor(n/2))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1, a(1) = 0 are special cases */
    if (n == 0)
        return (seq_t)1;     /* a(0) = 1 */
    if (n == 1)
        return (seq_t)0;     /* a(1) = 0 */

    /* k = floor(n/2) */
    idx_t k = n >> 1;          /* integer division by 2 */

    /* binomial coefficient */
    seq_t bin = binomial(n, k);

    /* Falling factorial (n‑1)! / (k‑1)! = product_{i=k}^{n‑1} i */
    seq_t ff = 1;
    for (idx_t i = k; i < n; ++i)
        ff *= (seq_t)i;

    /* Final result */
    return bin * ff;
}

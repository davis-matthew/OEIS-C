/*
 * A357671  a(n) = Sum_{k = 0..n} ( binomial(n+k-1,k) + binomial(n+k-1,k)^2 ).
 * https://oeis.org/A357671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n} ( binomial(n+k-1,k) + binomial(n+k-1,k)^2
 *            )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for the OEIS sequence A357671.
 * The computation uses only the integer operations allowed on seq_t
 * and relies on the arithmetic properties of the provided type.
 */
seq_t a(idx_t n)
{
    /* t = binomial(n + k - 1, k) for the current k. */
    seq_t t = 1;           /* k = 0 term: binomial(n-1, 0) == 1 */
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* Update t to binomial(n + k - 1, k) from the previous
             * binomial using the relation:
             *   C(n+k-1, k) = C(n+k-2, k-1) * (n + k - 1) / k
             */
            t *= (seq_t)(n + k - 1);
            t /= (seq_t)k;
        }

        /* Accumulate the terms: C + C^2 */
        sum += t;
        sum += t * t;
    }

    return sum;
}

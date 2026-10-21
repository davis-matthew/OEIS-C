/*
 * A293574  a(n) = Sum_{k=0..n} n^(n-k)*binomial(n+k-1,k).
 * https://oeis.org/A293574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} n^(n-k) * binomial(n+k-1, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Special case n == 0: 0^0 is treated as 1 */
    if (n == 0) return (seq_t)1;

    /* Compute n^n in a single loop (multiply by the small integer n each time) */
    seq_t term = 1;
    for (idx_t i = 0; i < n; ++i) {
        term *= (seq_t)n;
    }

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        sum += term;                    /* add current term to the running sum */

        if (k == n) break;              /* last iteration done */

        /* Update term to the next one:
         * term_{k+1} = term_k * (n + k) / (n * (k + 1))
         * Integer arithmetic is exact because the binomial factor guarantees divisibility.
         */
        term /= (seq_t)n;                /* divide by n       */
        term *= (seq_t)(n + k);          /* multiply by (n+k) */
        term /= (seq_t)(k + 1);          /* divide by (k+1)   */
    }

    return sum;
}

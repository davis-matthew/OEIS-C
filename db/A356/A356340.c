/*
 * A356340  a(n) = Sum_{k=1..n} binomial(2*n, n-k) * phi(k), where phi is the
 *          Euler totient function.
 * https://oeis.org/A356340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(2*n, n-k) * phi(k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset starts at 1, return 0 for safety */

    seq_t cur = binomial(2*n, n);            /* C(2n, n) */
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* update cur to C(2n, n-k) using the recurrence
           C(2n, n-k) = C(2n, n-(k-1)) * (n-k+1) / (n+k)   */
        cur = cur * (seq_t)(n - k + 1) / (seq_t)(n + k);

        /* add cur * phi(k) to the sum */
        sum += cur * phi(k);
    }

    return sum;
}

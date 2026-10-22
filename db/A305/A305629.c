/*
 * A305629  Number of values of k, 0 < k < n, for which (2*n^2 + 2*k*n - k^2 -
 *          k)/2 is prime.
 * https://oeis.org/A305629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of k (0 < k < n) such that (2*n^2 + 2*k*n - k^2 - k)/2
 *            is prime
 * Range:     u8 n <= 14, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* handle lack of admissible k */
    if (n < 2) return 0;

    seq_t count = 0;
    /* precompute n^2 once */
    seq_t nn = (seq_t)n * n;

    /* value for k = 1 */
    seq_t val = nn + (seq_t)n - 1;
    if (is_prime_big(val)) count++;

    /* iterate k = 2 … n-1 using incremental update:
       val(k+1) = val(k) + n - (k+1)  */
    for (idx_t k = 1; k + 1 < n; ++k) {
        val += (seq_t)n - (k + 1);
        if (is_prime_big(val)) count++;
    }

    return count;
}

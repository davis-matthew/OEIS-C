/*
 * A351857  Number of nonnegative integer solutions to n = x_1 + x_2 + ... +
 *          x_(2*n) + 2*y_1 + 2*y_2 + ... + 2*y_(2*n).
 * https://oeis.org/A351857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} C(3*n-2*k-1, n-2*k) * C(2*n+k-1, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* n>=1 by definition of the sequence; no safeguard needed */
    seq_t sum = 0;
    idx_t maxk = n / 2;            /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t t1 = binom(3 * n - 2 * k - 1, n - 2 * k);
        seq_t t2 = binom(2 * n + k - 1, k);
        sum += t1 * t2;            /* exact multiplication for arbitrary‑precision */
    }

    return sum;
}

/*
 * A351768  a(n) = n! * Sum_{k=0..n} k^(n-k) * (n-k)^k/k!.
 * https://oeis.org/A351768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(n-k) * (n-k)^k / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= i;

    seq_t sum = 0;
    seq_t r = fact;                         /* r = n! / k!   */

    for (idx_t k = 0; k <= n; ++k) {
        if (k != 0 && k != n) {             /* interior terms only   */
            seq_t term = r;
            term *= pow_seq((seq_t)k, (int)(n - k));
            term *= pow_seq((seq_t)(n - k), (int)k);
            sum += term;
        }
        if (k < n)                         /* prepare next r = n!/(k+1)! */
            r /= (k + 1);
    }

    return sum;
}

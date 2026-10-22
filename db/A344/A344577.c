/*
 * A344577  a(n) = Sum_{d|n, d<n} phi(n-d) for n >= 2.
 * https://oeis.org/A344577
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} phi(n-d)
 * Range:     u8 n <= 59, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"  /* provides seq_t phi(idx_t) or seq_t calc_phi(seq_t) */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset starts at n = 2 */

    seq_t sum = 0;
    idx_t i;
    for (i = 1; i <= n / i; ++i) {      /* iterate divisors i and n/i */
        if (n % i == 0) {
            /* divisor i */
            if (i < n) {
                seq_t val = phi((idx_t)(n - i));
                sum += val;
            }

            /* divisor n / i */
            idx_t d2 = n / i;
            if (d2 != i && d2 < n) {
                seq_t val2 = phi((idx_t)(n - d2));
                sum += val2;
            }
        }
    }
    return sum;
}

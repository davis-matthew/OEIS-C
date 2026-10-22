/*
 * A400413  a(n) = Sum_{k=1..n} (2^A007814(floor(n/k)) - 1).
 * https://oeis.org/A400413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = Sum_{k=1..n} (2^A007814(floor(n/k)) - 1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for n=0 */
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t q = n / k;            /* floor(n/k) */

        /* compute v2(q) */
        idx_t v = 0;
        while ((q & 1ULL) == 0 && q != 0) {
            ++v;
            q >>= 1;
        }

        /* term = 2^v - 1 */
        seq_t term = ((seq_t)1 << v) - 1;
        sum += term;
    }

    return sum;
}

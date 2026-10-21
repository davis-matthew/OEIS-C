/*
 * A284100  a(n) = Sum_{d|n, d == 1 (mod 8)} d.
 * https://oeis.org/A284100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d % 8 == 1} d
 * Range:     u8 n <= 224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* value for n = 0 is undefined; return 0 */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {   /* iterate i <= sqrt(n) without overflow */
        if (n % i == 0) {
            if ((i & 7) == 1) {          /* i ≡ 1 (mod 8) */
                sum += (seq_t)i;
            }
            idx_t j = n / i;            /* paired divisor */
            if (j != i && (j & 7) == 1) {
                sum += (seq_t)j;
            }
        }
    }
    return sum;
}

/*
 * A343132  a(n) is the quotient obtained when integer A343131(n) = k is
 *          divided by A061486(k).
 * https://oeis.org/A343132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A343131(n)/A061486(A343131(n))
 * Range:     u8 n <= 40, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t k = 1;
    idx_t count = 0;

    while (1) {
        /* compute f(k) = Π(1 + digit) - 1 */
        seq_t prod = 1;
        seq_t temp = k;
        while (temp > 0) {
            unsigned digit = (unsigned)(temp % 10);
            prod *= (seq_t)(digit + 1);
            temp /= 10;
        }
        seq_t f = prod - 1;

        if (k % f == 0) {
            ++count;
            if (count == n) {
                return k / f;
            }
        }
        ++k;
    }
}

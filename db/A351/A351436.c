/*
 * A351436  a(n) = n - A351168(n).
 * https://oeis.org/A351436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A351168(n), where A351168(n) = (n/p^e)*(p-1)^e, p =
 *            largest prime factor of n, e = its exponent; a(1) = 0
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    idx_t m = n;
    idx_t p = 1;
    unsigned int e = 0;

    for (idx_t f = 2; f <= m / f; f++) {
        if (m % f == 0) {
            unsigned int c = 0;
            while (m % f == 0) {
                m /= f;
                c++;
            }
            p = f;
            e = c;
        }
    }
    if (m > 1) {
        p = m;
        e = 1;
    }

    seq_t rest = (seq_t)n;
    seq_t mult = (seq_t)1;
    for (unsigned int i = 0; i < e; i++) {
        rest /= (seq_t)p;
        mult *= (seq_t)(p - 1);
    }
    return (seq_t)n - rest * mult;
}

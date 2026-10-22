/*
 * A334888  a(n) = Sum_{k >= 0} f(d_k) * 3^k where Sum_{k >= 0} d_k * 8^k is
 *          the base 8 representation of n and f(k) = 0, 1, 2, 2, 2, 1, 0, 0
 *          for k = 0..7, respectively.
 * https://oeis.org/A334888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k>=0} f(d_k) * 3^k where d_k are the octal digits of
 *            n and f = {0,1,2,2,2,1,0,0} for k = 0..7
 * Range:     u8 n <= 4095, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const unsigned char f[8] = {0, 1, 2, 2, 2, 1, 0, 0};
    seq_t result = 0;
    seq_t mult   = 1;          /* 3^k */
    while (n) {
        unsigned digit = (unsigned)(n & 7ULL);   /* n % 8 */
        result += (seq_t)f[digit] * mult;
        mult *= 3;
        n >>= 3;                       /* n /= 8 */
    }
    return result;
}

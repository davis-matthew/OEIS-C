/*
 * A376718  Expansion of (1 - x + x^4)/((1 - x + x^4)^2 - 4*x^4).
 * https://oeis.org/A376718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 2*a(n-1) - a(n-2) + 2*a(n-4) + 2*a(n-5) - a(n-8),
   a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=4, a(5)=11, a(6)=22, a(7)=37 */
#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t init[8] = {1, 1, 1, 1, 4, 11, 22, 37};
    if (n < 8)
        return init[(size_t)n];

    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t val = buf[(size_t)((i - 1) & 7)];
        val = val + val;                                               /* 2*a(i-1)        */
        val = val + (buf[(size_t)((i - 4) & 7)] + buf[(size_t)((i - 4) & 7)]); /* +2*a(i-4)  */
        val = val + (buf[(size_t)((i - 5) & 7)] + buf[(size_t)((i - 5) & 7)]); /* +2*a(i-5)  */
        val = val - buf[(size_t)((i - 2) & 7)];                         /* -a(i-2)         */
        val = val - buf[(size_t)((i - 8) & 7)];                         /* -a(i-8)         */
        buf[(size_t)(i & 7)] = val;
    }
    return buf[(size_t)(n & 7)];
}

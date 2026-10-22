/*
 * A345029  a(n) = Sum_{k=1..n} 3^(floor(n/k) - 1).
 * https://oeis.org/A345029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 3^(floor(n/k)-1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */
    seq_t sum  = 0;
    idx_t  i   = 1;

    /* Process ranges where floor(n/k) is constant.
       This gives an O(√n) algorithm. */
    while (i <= n) {
        idx_t v     = n / i;            /* current floor(n/k) */
        idx_t next  = n / v;            /* last k with this value */
        idx_t count = next - i + 1;     /* number of k in this block */

        seq_t pw    = big_pow((seq_t)3, (seq_t)(v - 1)); /* 3^(v-1) */
        sum = sum + pw * (seq_t)count;                   /* block contribution */

        i = next + 1;
    }

    return sum;
}

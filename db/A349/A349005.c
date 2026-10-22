/*
 * A349005  a(n) = Sum_{d|n, d^2>=n} 1+d+n/d.
 * https://oeis.org/A349005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d^2 >= n} (1 + d + n/d)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    /* returns ⌊√n⌋ for n >= 1 */
    if (n < 2) return n; /* sqrt(0)=0, sqrt(1)=1 */

    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        /* mid > 0 because n >= 2 */
        if (mid <= n / mid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t sqrt_n = isqrt_u64(n);
    seq_t result = 0;

    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;          /* the paired divisor */
            if (i == j) {             /* perfect square divisor */
                result += (seq_t)1 + (seq_t)2 * (seq_t)i;
            } else {                  /* j > sqrt, contributes once */
                result += (seq_t)1 + (seq_t)j + (seq_t)i;
            }
        }
    }
    return result;
}

/*
 * A277975  a(n) = n*x^n + (n-1)*x^(n-1) + . . . + x + 1 for x=5.
 * https://oeis.org/A277975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Σ_{k=1}^{n} k*5^k (equivalently, a(0)=1;
 *            a(n)=a(n-1)+n*5^n for n≥1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 1;   /* a(0) */
    seq_t power  = 1;   /* current 5^k, starts at 5^0 */
    for (idx_t k = 1; k <= n; ++k) {
        power *= (seq_t)5;                /* power = 5^k */
        result += (seq_t)k * power;      /* add k*5^k */
    }
    return result;
}

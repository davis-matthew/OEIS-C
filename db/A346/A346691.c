/*
 * A346691  Replace 7^k with (-1)^k in base-7 expansion of n.
 * https://oeis.org/A346691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = n + 8 * Σ_{k≥1} (-1)^k * ⌊n/7^k⌋
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t pow = 7;
    int sign = -1;                /* (+) for least‑significant digit */
    while (pow <= n) {
        idx_t q = n / pow;       /* ⌊n/7^k⌋  */
        if (q == 0) break;
        sum += (seq_t)sign * (seq_t)q;
        sign = -sign;
        if (pow > n / 7) break; /* prevent overflow when multiplying by 7 */
        pow *= 7;
    }
    return (seq_t)n + 8 * sum;
}

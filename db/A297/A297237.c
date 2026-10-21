/*
 * A297237  Down-variation of the base-13 digits of n; see Comments.
 * https://oeis.org/A297237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{i>=1} max(0, d_i - d_{i-1}) for base‑13 digits d_i of
 *            n (d_0 is the least significant digit)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe for offset 1, but not used */
    seq_t res = 0;
    seq_t prev = (seq_t)(n % 13);
    n /= 13;
    while (n > 0) {
        seq_t cur = (seq_t)(n % 13);
        if (cur > prev)
            res += cur - prev;
        prev = cur;
        n /= 13;
    }
    return res;
}

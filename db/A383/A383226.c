/*
 * A383226  Number of winning positions for the next player (a, b, c) where 1
 *          <= a, b, c <= n in "Divisor Nim" (see comments).
 * https://oeis.org/A383226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{0 <= a,b,c <= floor(log_2(n))} g(a,b,c), where
 *            g(a,b,c) = 0 if a XOR b XOR c = 0, else g(a,b,c) =
 *            f(a)*f(b)*f(c); f(x) = floor(n/2^x) - floor(n/2^(x+1)). This
 *            counts the winning positions for the next player in Divisor Nim
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute floor(log2(n)) */
    idx_t k = 0;
    idx_t pow = 1;
    while (pow <= n) {
        pow <<= 1;
        ++k;
    }
    if (k > 0) --k;           /* now 2^k <= n < 2^(k+1) */

    /* f[0..k] */
    seq_t f[64];
    for (idx_t x = 0; x <= k; ++x) {
        seq_t t1 = (seq_t)(n >> x);
        seq_t t2 = (seq_t)(n >> (x + 1));
        f[x] = t1 - t2;
    }

    /* sum over all triples where XOR != 0 */
    seq_t ans = 0;
    for (idx_t a = 0; a <= k; ++a) {
        for (idx_t b = 0; b <= k; ++b) {
            for (idx_t c = 0; c <= k; ++c) {
                if ((a ^ b ^ c) != 0) {
                    ans += f[a] * f[b] * f[c];
                }
            }
        }
    }
    return ans;
}

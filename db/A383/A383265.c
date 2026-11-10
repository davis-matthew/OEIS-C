/*
 * A383265  a(n) = Sum_{k=0..n} A383266(n, k).
 * https://oeis.org/A383265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 + n + Sum_{k=2..n} floor(log_k(n)), for n >= 1; a(0)
 *            = 0
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t N = (seq_t)n;
    seq_t total = N * N + N;
    idx_t k = 2;
    /* k with k*k <= n: compute exponent directly */
    while (k <= n / k) {
        idx_t p = 1;
        idx_t e = 0;
        while (p <= n / k) {
            p *= k;
            e++;
        }
        total += (seq_t)e;
        k++;
    }
    /* remaining k in [k, n] each contribute exactly 1 */
    if (k <= n) total += (seq_t)(n - k + 1);
    return total;
}

/*
 * A332598  Number of vertices in a "frame" of size n X n (see Comments in
 *          A331776 for definition).
 * https://oeis.org/A332598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(n-1)*(3*n-1)+12*sum_{i=2..floor(n/2)}
 *            (n+1-i)*i*phi(i)+4*sum_{i=floor(n/2)+1..n}
 *            (n+1-i)*(2*n+2-i)*phi(i); a(1)=5, a(2)=27
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 310, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)27;

    idx_t half = n >> 1;   /* floor(n/2) */
    seq_t sum1 = 0;
    seq_t sum2 = 0;

    for (idx_t i = 2; i <= half; ++i) {
        seq_t phi_i = phi(i);
        seq_t term = (seq_t)(n + 1 - i) * (seq_t)i;
        term *= phi_i;
        sum1 += term;
    }

    for (idx_t i = half + 1; i <= n; ++i) {
        seq_t phi_i = phi(i);
        seq_t term = (seq_t)(n + 1 - i) *
                     (((seq_t)2 * (seq_t)n) + 2 - (seq_t)i);
        term *= phi_i;
        sum2 += term;
    }

    seq_t base = (seq_t)4 *
                 ((seq_t)(n - 1) *
                  (((seq_t)3 * (seq_t)n) - 1));

    seq_t result = base + (seq_t)12 * sum1 + (seq_t)4 * sum2;
    return result;
}

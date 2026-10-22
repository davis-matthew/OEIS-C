/*
 * A396670  Binary colorings of the sunlet graph S_n up to rotation, with no
 *          monochromatic connected component of size 3 or more.
 * https://oeis.org/A396670
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} (phi(n/d) * ((1+sqrt(2))^d +
 *            (1-sqrt(2))^d + 2*(-1)^d))
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t phi_small(idx_t m) {
    idx_t res = m;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            res -= res / p;
        }
    }
    if (m > 1) res -= res / m;
    return (seq_t)res;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;   /* undefined below the offset */

    seq_t a_prev2 = (seq_t)2; /* a0 */
    seq_t a_prev1 = (seq_t)2; /* a1 */
    seq_t sum = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t a_i;
        if (i == 1) {
            a_i = a_prev1;
        } else {
            a_i = (seq_t)2 * a_prev1 + a_prev2;
            a_prev2 = a_prev1;
            a_prev1 = a_i;
        }

        if (n % i == 0) {
            seq_t phi_val = phi_small(n / i);
            seq_t expr = a_i + ((i & 1) ? (seq_t)-2 : (seq_t)2);
            sum += phi_val * expr;
        }
    }

    return sum / (seq_t)n;
}

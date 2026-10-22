/*
 * A332682  a(n) = Sum_{k=1..n} (-1)^(k+1) * ceiling(n/k).
 * https://oeis.org/A332682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * ceil(n/k)
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute A332682.  n ≥ 1.  The function is fast enough for
 * n up to at least 10 000 with an O(n) algorithm.  The sum
 * is always positive, so we safely use unsigned arithmetic
 * and subtract when the sign is negative. */
seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* ceil(n/k) = (n + k - 1) / k */
        seq_t term = (seq_t)(n + k - 1) / (seq_t)k;
        if (k & 1) {          /* odd k → positive sign */
            sum += term;
        } else {              /* even k → negative sign */
            sum -= term;      /* sum stays non‑negative */
        }
    }
    return sum;
}

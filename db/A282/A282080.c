/*
 * A282080  Number of n-element subsets of [n+4] having an even sum.
 * https://oeis.org/A282080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of n‑element subsets of [n+4] having an even sum.
 *            Computed via parity generating function: a(n) = (binom(n+4, n) +
 *            S)/2 where N = n + 4 If N even: e = N/2, o = e If n odd S = 0 If
 *            n even let m = n/2 S = (-1)^m * binom(e, m) If N odd: e = N/2, o
 *            = e + 1 If n even let m = n/2 S = (-1)...
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 399, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    idx_t N = n + 4ULL;          /* total cardinality of the set */
    seq_t total = binom(N, n);   /* total number of n‑subsets */

    if (N & 1ULL) { /* N is odd */
        idx_t e = N >> 1;       /* floor(N/2) */
        if ((n & 1ULL) == 0ULL) {           /* n even */
            idx_t m = n >> 1;
            seq_t mag = binom(e, m);
            if ((m & 1ULL) == 0ULL) {
                /* sign = +1 if m even, -1 otherwise */
                total += mag;
            } else {
                total -= mag;
            }
        } else { /* n odd */
            idx_t m = (n - 1ULL) >> 1;
            seq_t mag = binom(e, m);
            if ((m & 1ULL) == 0ULL) {
                /* sign = -1 if m even, +1 otherwise */
                total -= mag;
            } else {
                total += mag;
            }
        }
    } else { /* N is even */
        idx_t e = N >> 1;          /* e = o */
        if ((n & 1ULL) == 0ULL) {   /* n even, S non‑zero */
            idx_t m = n >> 1;
            seq_t mag = binom(e, m);
            if ((m & 1ULL) == 0ULL) {
                total += mag;
            } else {
                total -= mag;
            }
        } /* else n odd => S = 0, nothing to add */
    }

    return total / 2ULL;
}

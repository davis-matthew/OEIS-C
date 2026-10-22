/*
 * A307744  A fractal function, related to ruler functions and the Cantor set.
 *          a(1) = 0; for m >= 0, a(3m) = 1; for m >= 1, a(3m-1) = a(m-1) +
 *          sign(a(m-1)), a(3m+1) = a(m+1) + sign(a(m+1)).
 * https://oeis.org/A307744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 1, a(1) = 0, a(3m) = 1, a(3m-1) = a(m-1) + sign(a(m-1)),
 *            a(3m+1) = a(m+1) + sign(a(m+1)) where sign(x) = 0 if x==0, else
 *            1
 * Range:     u8 n <= 4374, u16 n <= 4374, u32 n <= 4374, u64 n <= 4374,
 *            u128 n <= 4374, big any n
 * Helpers:   sign_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/sign_seq_t.h"

seq_t a(idx_t n) {
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    idx_t r = n % 3U;  /* n mod 3 */
    if (r == 0) {
        return (seq_t)1;
    } else if (r == 1) {
        /* n = 3m + 1 */
        idx_t m = (n - 1U) / 3U;
        seq_t t = a(m + 1U);   /* a(m+1) */
        return t + sign_seq_t(t);
    } else { /* r == 2 */
        /* n = 3m - 1 */
        idx_t m = (n + 1U) / 3U;
        seq_t t = a(m - 1U);   /* a(m-1) */
        return t + sign_seq_t(t);
    }
}

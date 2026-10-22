/*
 * A320312  Number of ways to write n as the sum of 4 positive integers a, b,
 *          c, d such that b != d and a/b - c/d = (a - c)/(b - d).
 * https://oeis.org/A320312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4‑tuples (a,b,c,d) of positive integers with
 *            a+b+c+d = n, b != d, and a/b – c/d = (a – c)/(b – d)
 * Range:     u8 n <= 17, u16 n <= 91, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function enumerates (b,d) pairs with b<2d and b!=d, solves for
   c and a using the derived equation
     a d^2 = b c (2d – b)
   and the sum constraint. */

#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t count = 0;
    if (n < 4) return 0;  /* minimum sum of 4 positive integers is 4 */

    for (idx_t d = 1; d <= n - 3; ++d) {
        idx_t max_b = 2 * d - 1;          /* b < 2d */
        if (max_b > n - d - 1)            /* ensure n > b + d + 1 for positivity of a later */
            max_b = n - d - 1;
        for (idx_t b = 1; b <= max_b; ++b) {
            if (b == d) continue;        /* b != d */
            idx_t nbd = n - d - b;
            if (nbd == 0) continue;      /* would give N<=0 */

            /* Compute N = d^2 * (n - d - b) */
            seq_t dd = (seq_t)d * d;
            seq_t N = dd * (seq_t)nbd;

            /* Compute D = d^2 + 2*b*d - b^2 */
            seq_t D = dd + (seq_t)2 * b * d - (seq_t)b * b;
            if (D == 0) continue;        /* should not happen for b<2d */

            if ((N % D) != 0) continue;  /* c must be integer */
            seq_t c = N / D;
            if (c == 0) continue;

            seq_t a_val = (seq_t)n - (seq_t)b - c - (seq_t)d;
            if (a_val == 0) continue;    /* ensures a>0 */

            /* All conditions satisfied; count this quadruple */
            count += 1;
        }
    }
    return count;
}

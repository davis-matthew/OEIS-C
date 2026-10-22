/*
 * A349074  a(n) = U(3*n, n), where U(n, x) is the Chebyshev polynomial of the
 *          second kind.
 * https://oeis.org/A349074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = U(3*n, n), where U is the Chebyshev polynomial of the
 *            second kind
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = U(3*n, n) via the recurrence
 *   U(0, x) = 1
 *   U(1, x) = 2*x
 *   U(k+1, x) = 2*x*U(k, x) - U(k-1, x)
 */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Special case n == 0 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Number of recurrence steps: m = 3*n */
    idx_t m = 3 * n;
    seq_t x = (seq_t)n;          /* evaluation point */

    /* Initial values of the recurrence */
    seq_t U_prev2 = (seq_t)1;                 /* U(0, x) */
    seq_t U_prev1 = ((seq_t)2) * x;           /* U(1, x) */

    /* Run recurrence up to U(m, x) */
    for (idx_t i = 2; i <= m; ++i) {
        seq_t U_cur = ((seq_t)2) * x * U_prev1 - U_prev2;
        U_prev2 = U_prev1;
        U_prev1 = U_cur;
    }

    /* U_prev1 now holds U(3*n, n) */
    return U_prev1;
}

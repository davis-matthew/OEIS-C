/*
 * A364517  a(n) = (5/7) * (9*n)!*(7*n/2)!^2/((9*n/2)!*(7*n)!*(5*n/2)!*n!^2)
 *          for n >= 1, with a(0) = 1.
 * https://oeis.org/A364517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(7*n-1, n-k)^2 * binomial(5*n + k -
 *            2, k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = 0;
    seq_t bc = 1;          /* current C(7*n-1, n-k)   */
    seq_t A  = 1;          /* current C(5*n + k - 2, k) */

    idx_t seven_n_minus_1 = 7 * n - 1;   /* 7*n - 1 */

    /* Compute bc = C(7*n-1, n) via a multiplicative recurrence */
    seq_t cur = 1;
    for (idx_t i = 1; i <= n; ++i) {
        /* cur = cur * (7*n-1+1-i) / i  */
        cur = cur * ((seq_t)(seven_n_minus_1 + 1 - i)) / (seq_t)i;
    }
    bc = cur;

    for (idx_t k = 0; k <= n; ++k) {
        /* term = bc^2 * A */
        seq_t term = bc * bc;
        term = term * A;
        res += term;

        if (k < n) {
            /* Update bc to C(7*n-1, n-(k+1)) */
            idx_t r = n - k;  /* current r = n-k   */
            seq_t num_bc = (seq_t)r;
            seq_t den_bc = (seq_t)(seven_n_minus_1 - r + 1);
            bc = bc * num_bc / den_bc;

            /* Update A to C(5*n + (k+1) - 2, k+1) */
            seq_t num_A = (seq_t)(5 * n + k - 1);
            seq_t den_A = (seq_t)(k + 1);
            A = A * num_A / den_A;
        }
    }

    return res;
}

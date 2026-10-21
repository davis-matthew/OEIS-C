/*
 * A252355  a(n) = sum_{k = 0..n-1} (-1)^k*C(2*n-1,k)*C(n-1,k), n>0.
 * https://oeis.org/A252355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sum_{k = 0..n-1} (-1)^k*C(2*n-1,k)*C(n-1,k)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 13, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* Handle the first two values directly */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)-2;

    /* Use the given 3‑term linear recurrence:
     * 2*(n-1)*(2*n-1)*(7*n-11)*a(n) =
     * -(91*n^3 - 325*n^2 + 368*n - 128)*a(n-1)
     * - 16*(n-2)*(2*n-3)*(7*n-4)*a(n-2)
     *   for n >= 3.
     */
    seq_t a_prev2 = (seq_t)1;   /* a(1) */
    seq_t a_prev1 = (seq_t)-2;  /* a(2) */
    seq_t a_curr;

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute integer coefficients using 64‑bit arithmetic */
        uint64_t ii = i;
        int64_t coeff1 = -(int64_t)(91ULL * ii * ii * ii
                                 - 325ULL * ii * ii
                                 + 368ULL * ii
                                 - 128ULL);
        int64_t coeff2 = -(int64_t)(16ULL * (ii - 2ULL)
                                 * (2ULL * ii - 3ULL)
                                 * (7ULL * ii - 4ULL));
        seq_t denom = (seq_t)2 * (ii - 1ULL) * (2ULL * ii - 1ULL) * (7ULL * ii - 11ULL);

        seq_t num = (seq_t)coeff1 * a_prev1 + (seq_t)coeff2 * a_prev2;
        a_curr = num / denom;   /* exact division */

        /* Shift the window for the next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}

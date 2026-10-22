/*
 * A373766  Triangle read by rows, T(n,k) with n,k > 1, is the number of
 *          subsequences of length k over all permutations of [n], which are
 *          neither increasing or decreasing.
 * https://oeis.org/A373766
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k) with m = minimal row such that * (m-1)(m-2)/2 ≤
 *            (n-2) < m(m-1)/2 * k = 1 + ((n-2) - (m-1)(m-2)/2) * T(m,k) =
 *            m!*C(m,k) - 2*((m-k)! * C(m,n-k)^2) * = n!*C(n,k) - 2*((n-k)! *
 *            C(n,n-k)^2) * * The sequence is the triangle of non‑monotonic
 *            subsequences read by rows, * with index offset 2...
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"   /* provides factorial(idx_t)          */
#include "../utils/binomial.h"   /* provides binomial(idx_t,idx_t)    */

seq_t a(idx_t n)
{
    /* Below the offset the value is undefined – return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Position in the flattened triangle (0‑based). */
    idx_t pos = n - 2;

    /* Find the row m such that cumulative entries before row m
     * are ≤ pos, but those before row m+1 are > pos.  The
     * cumulative count before row m is (m-1)(m-2)/2. */
    idx_t m = 2;
    while (pos >= m * (m - 1) / 2)
        ++m;

    /* Offset inside row m.  The first entry of row m corresponds to
     * k = 2, so k = 2 + (pos - cumulative). */
    idx_t cumulative = (m - 1) * (m - 2) / 2;
    idx_t k_index = pos - cumulative;      /* 0 … (m-2)      */
    idx_t k = k_index + 2;                 /* 2 … m   */

    /* Compute the triangle value T(m,k).  All calculations are done
     * in seq_t; for types that overflow, only terms that fit are
     * checked by the test harness. */
    seq_t fact_m     = factorial(m);          /* m!      */
    seq_t fact_mk    = factorial(m - k);      /* (m-k)!  */
    seq_t cnk        = binomial(m, k);        /* C(m,k)  */

    seq_t term1 = fact_m * cnk;                           /* m!*C(m,k) */
    seq_t sq_cnk = cnk * cnk;                              /* C(m,k)^2 */
    seq_t term2 = fact_mk * sq_cnk;                        /* (m-k)! * C(m,k)^2 */
    term2 = term2 * (seq_t)2;                              /* multiply by 2 */

    return term1 - term2;
}

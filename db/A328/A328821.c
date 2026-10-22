/*
 * A328821  Triangular array read by rows. Let P be the poset of all even
 *          sized subsets of [2n] ordered by inclusion. T(n,k) is the number
 *          of intervals in P with length k, 0<=k<=n, n>=0.
 * https://oeis.org/A328821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangular array of interval counts * in
 *            the poset of even‑size subsets of [2r]. For 0 ≤ k ≤ r, * T(r,k)
 *            = C(2r,2k) * 2^(2(r-k)-1) for k < r * = 1 for k = r * The
 *            sequence a(n) is obtained by reading the rows of T(r,·)
 *            left‑to‑right: * a(n) = T(r,k) where r is the unique int...
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/pow2.h"    /* seq_t pow2(idx_t exp);     */

static seq_t binom_multiplicative(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* multiply then divide – the division is exact */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    idx_t prefix = r * (r + 1) / 2;   /* first index of row r   */
    idx_t k = n - prefix;             /* column within the row */

    if (k == r)              /* interval of maximal length */
        return (seq_t)1;

    /* T(r,k) = C(2r,2k) * 2^(2(r-k)-1) */
    seq_t comb = binom_multiplicative(2 * r, 2 * k);
    seq_t pow2_exp = pow2(2 * (r - k) - 1);
    return comb * pow2_exp;
}

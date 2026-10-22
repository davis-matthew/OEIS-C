/*
 * A323231  A(n, k) = [x^k] (1/(1-x) + x/(1-x)^n), square array read by
 *          descending antidiagonals for n, k >= 0.
 * https://oeis.org/A323231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array A(n,k) = read by
 *            descending antidiagonals. A(n,k) = binomial(n + k - 2, k - 1) +
 *            1 (for n,k ≥ 0, A(n,0)=1, with the special exceptional value
 *            A(0,1)=2)
 * Range:     u8 n <= 95, u16 n <= 239, u32 n <= 720, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* ------------------------------------------------------------ */
/* Helper: largest d such that d*(d+1)/2 <= idx                    */
static idx_t diag_of(idx_t idx)
{
    /* estimate of the correct d using the inverse quadratic formula */
    long double est = sqrt((long double)(2.0L * idx + 1.0L));
    idx_t lo = 0;
    idx_t hi = (idx_t)(est);          /* hi is a safe over‑estimate    */

    /* binary search for the exact largest d */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 tr = (unsigned __int128)mid * (mid + 1) / 2;
        if (tr <= idx)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ------------------------------------------------------------ */
/* Compute binomial(n,k) with the property that the intermediate
   values never overflow 128 bits.  For n<k, returns 0.           */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        unsigned __int128 prod = (unsigned __int128)result *
                                 (unsigned __int128)(n - k + i);
        prod /= (unsigned __int128)i;
        result = (seq_t)prod;
    }
    return result;
}

/* ------------------------------------------------------------ */
/* Main sequence function                                            */
seq_t a(idx_t n)
{
    /* find diagonal index d and position j on that diagonal   */
    idx_t d = diag_of(n);
    unsigned __int128 pref128 = (unsigned __int128)d * (d + 1) / 2;
    idx_t pref = (idx_t)pref128;
    idx_t j = n - pref;                 /* 0 ≤ j ≤ d */

    idx_t row = j;          /* n in the A(n,k) definition   */
    idx_t col = d - j;      /* k in the A(n,k) definition   */

    /* exceptional values   */
    if (col == 0)
        return (seq_t)1;              /* A(n,0)=1 for all n        */
    if (row == 0 && col == 1)
        return (seq_t)2;              /* A(0,1)=2                   */

    seq_t val = binom(row + col - 2, col - 1);
    return val + (seq_t)1;            /* A(n,k)=binom+1              */
}

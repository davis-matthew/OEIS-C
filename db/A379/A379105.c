/*
 * A379105  Triangular array read by rows. T(n,k) is the number of n X n
 *          matrices T over GF(2) such that there are exactly 2^k vectors v in
 *          GF(2)^n with Tv=v, n>=0, 0<=k<=n.
 * https://oeis.org/A379105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 20, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A379105:  Triangular array read by rows.  
 *  T(n,k) = number of n×n matrices over GF(2) with nullity k.
 *  Formula:  T(n,k) = ∏_{j=0}^{n-k-1} (2^n−2^j)^2 / (2^{\,n-k}−2^j)   (0≤k≤n)
 *  a(n) is obtained by reading the triangular array T row by row. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow2.h"          /* seq_t pow2(idx_t exp); */
#include "../utils/gcd.h"           /* seq_t gcd(seq_t a, seq_t b); */

static idx_t find_row(idx_t linear)
{
    /* Find largest r such that r*(r+1)/2 <= linear. */
    idx_t lo = 0, hi = 1;
    while (hi * (hi + 1) / 2 <= linear) hi <<= 1;  /* fast doubling */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid * (mid + 1) / 2 <= linear)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t T_n_k(idx_t n, idx_t k)
{
    idx_t r = n - k;                     /* rank */
    if (r == (idx_t)0) return 1;        /* nullity n => zero matrix */

    seq_t num = 1;
    seq_t den = 1;
    seq_t pow2_n   = pow2(n);
    seq_t pow2_nk  = pow2(r);            /* n-k = r */

    for (idx_t j = 0; j < r; ++j) {
        seq_t pow2_j   = pow2(j);
        seq_t term     = pow2_n - pow2_j;          /* (2^n - 2^j)   */
        seq_t term_sq  = term * term;              /* square it     */
        seq_t den_term = pow2_nk - pow2_j;         /* (2^k   - 2^j) */

        /* Reduce term_sq and den_term before multiplying the running product. */
        seq_t g1 = gcd(term_sq, den_term);
        if (g1 > 1) {
            term_sq  /= g1;
            den_term /= g1;
        }

        /* Multiply into the numerator and denominator. */
        num *= term_sq;
        den *= den_term;

        /* Reduce num and den whenever possible. */
        seq_t g2 = gcd(num, den);
        if (g2 > 1) {
            num /= g2;
            den /= g2;
        }
    }

    /* At the end den should be 1; return the reduced numerator. */
    return num / den;
}

seq_t a(idx_t n)
{
    idx_t row = find_row(n);
    idx_t col = n - row * (row + 1) / 2;
    return T_n_k(row, col);
}

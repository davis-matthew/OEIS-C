/*
 * A355282  Triangle read by rows: T(n, k) = Sum_{i=1..n-k} qStirling1(n-k, i)
 *          * qStirling2(n-1+i, n-1) for 0 < k < n with initial values T(n, 0)
 *          = 0^n and T(n, n) = 1 for n >= 0, here q = 2.
 * https://oeis.org/A355282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,c) where T is the triangle of OEIS A355282. T(r,0) =
 *            0^r (0 if r>0, 1 if r==0), T(r,r) = 1. For 0 < k <= r: T(r,k) =
 *            Σ_{i=0..r-k} (-1)^i * binom(r-1,i) * [r-1-i, k-1]_2 *
 *            2^((r-k)*(r-k-i)), where [x,y]_2 is the Gaussian binomial
 *            coefficient (q‑binomial) with q=2 (provided below). a(n)...
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 28, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/pow2.h"    /* seq_t pow2(idx_t exp); */
#include "../utils/seq_pow.h" /* seq_t seq_pow(seq_t base, idx_t exp); */

/* Gaussian binomial coefficient (q=2).  Implements
     [n k]_2 = ∏_{j=1..k} (2^{n-k+j} - 1) / (2^j - 1)    */
static seq_t qgbinom2(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t j = 1; j <= k; ++j) {
        seq_t numer = pow2(n - k + j) - 1;
        seq_t denom = pow2(j) - 1;
        res = (res * numer) / denom;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* locate (r,k) from linear index n : 0‑based */
    idx_t r = 0;
    while (((r + 1) * (r + 2) / 2) <= n) ++r;
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;   /* column k in row r */

    /* base cases */
    if (r == 0) return 1;    /* T(0,0)=1 */
    if (c == 0) return 0;    /* T(r,0)=0 for r>0 */
    if (c == r) return 1;    /* T(r,r)=1 */

    /* compute the alternating sum */
    idx_t diff = r - c;      /* r-k */
    seq_t pos_sum = 0, neg_sum = 0;
    seq_t binom = 1;         /* binom(r-1,0) */

    for (idx_t i = 0; i <= diff; ++i) {
        /* Gaussian coefficient [r-1-i , c-1]_2 */
        seq_t gauss = qgbinom2(r - 1 - i, c - 1);

        /* exponent for the power of two */
        idx_t exp2 = diff * (diff - i);
        seq_t two_pow = pow2(exp2);

        /* term = binom * gauss * 2^exp2 */
        seq_t term = binom * gauss;
        term = term * two_pow;

        /* add to the appropriate sum */
        if ((i & 1) == 0)
            pos_sum = pos_sum + term;
        else
            neg_sum = neg_sum + term;

        /* update binom for next i (if any) */
        if (i < diff) {
            /* binom(r-1,i+1) = binom(r-1,i)*(r-1-i)/(i+1) */
            binom = (binom * ((r - 1) - i)) / (i + 1);
        }
    }

    /* the triangle entries are non‑negative; pos_sum >= neg_sum */
    return pos_sum - neg_sum;
}

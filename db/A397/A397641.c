/*
 * A397641  Irregular triangle read by rows: T(m,r) is the number of chemical
 *          reaction networks on species A and B that use both species,
 *          contain r reactions, and have only complexes i*A+j*B with 1 <= i+j
 *          <= m, up to swapping A and B.
 * https://oeis.org/A397641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 28, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This implements the OEIS entry A397641.  For an index n (1‑based) the
   function locates the row m and position r of the irregular triangle
   defined by 
        T(m,r) = (C(R,r) - 2·C(d,r) + coeff(r))/(2) ,
   where C(x,y) is the binomial coefficient, R = c·(c-1), c = m(m+3)/2,
   d = m(m-1), and the coefficient term is the coefficient of x^r in
   (1+x)^f·(1+x²)^p with f = ⌊m/2⌋(⌊m/2⌋−1) and p = (R−f)/2.
   The sequence is the concatenation of all rows m=1,2,3,...
*/

#include <stdbool.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/seq_mul.h"     /* seq_t seq_mul(seq_t a, seq_t b); */

/* Computes the n-th term of OEIS sequence A397641. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n<1, return 0 */

    /* Find the row m and the relative index r within that row. */
    idx_t m = 1;
    idx_t cumul = 0;                     /* cumulative length up to previous row */
    while (1) {
        idx_t c   = m * (m + 3) / 2;
        idx_t R_idx = c * (c - 1);
        if (cumul + R_idx >= n) break;
        cumul += R_idx;
        ++m;
    }
    idx_t r = n - cumul;                 /* 1‑based r within the row */

    /* Pre‑compute constants. */
    idx_t c   = m * (m + 3) / 2;
    idx_t R_idx = c * (c - 1);           /* integer R for loop bounds */
    idx_t d   = m * (m - 1);
    idx_t h   = m / 2;
    idx_t f   = h * (h - 1);
    idx_t p   = (R_idx - f) / 2;

    /* T(m,r) = (C(R,r) - 2*C(d,r) + coeff)/2 */
    seq_t value = binomial(R_idx, r);

    seq_t two_bin_d = binomial(d, r);
    two_bin_d = seq_mul(two_bin_d, 2);   /* multiply by 2 */
    value = value - two_bin_d;

    /* Compute the coefficient of x^r in (1+x)^f*(1+x^2)^p */
    seq_t coeff = 0;
    idx_t maxk = (f < r ? f : r);
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t rest = r - k;
        if (rest & 1) continue;          /* r-k must be even */
        idx_t t = rest / 2;
        if (t > p) continue;
        seq_t ck = binomial(f, k);
        seq_t pt = binomial(p, t);
        seq_t prod = seq_mul(ck, pt);
        coeff = coeff + prod;
    }
    value = value + coeff;

    /* Final division by 2. */
    value = value / 2;
    return value;
}

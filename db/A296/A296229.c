/*
 * A296229  Triangle read by rows: Eulerian triangle that produces sums of
 *          even powers.
 * https://oeis.org/A296229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,j) where m and j satisfy n = m(m−1)/2 + j , 1 ≤ j ≤
 *            m; with T(m,j) = Σ_{i=1..j} (−1)^{j−i} * binomial(m+1, j−i) *
 *            (2i)^m
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binomial, pow_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Offset: 1  (a(1) is the first defined term) */

#include "../utils/binomial.h"
#include "../utils/pow_simple.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1 */
    if (n == 0) return 0;          /* not expected, but safe */

    /* Find the row m such that m(m+1)/2 ≥ n  */
    idx_t m = 1;
    while (m * (m + 1) / 2 < n) ++m;

    /* Elements before the current row */
    idx_t before = (m - 1) * m / 2;
    /* Position inside the row (1‑based) */
    idx_t j = n - before;

    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t i = 1; i <= j; ++i) {
        idx_t k = j - i;                     /* exponent of (−1)  */
        seq_t bin = binomial(m + 1, k);       /* C(m+1, k)        */
        seq_t pw = pow_simple((seq_t)(2 * i), m); /* (2i)^m           */
        seq_t term = bin * pw;                /* term of the sum   */

        if ((k & 1) == 0) {
            pos += term;                      /* positive sign      */
        } else {
            neg += term;                      /* negative sign      */
        }
    }
    return pos - neg;   /* T(m,j) is non‑negative for all m,j */
}

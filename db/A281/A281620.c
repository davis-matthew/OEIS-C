/*
 * A281620  Triangle read by rows: Poincaré polynomials of orbifold of Fermat
 *          hypersurfaces.
 * https://oeis.org/A281620
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n−2)-th entry of the triangle of Poincaré
 *            polynomials * (OEIS A281620). Each row r≥2 contains the
 *            coefficients of the polynomial * of degree r−2 written from
 *            constant term to leading term. The function * maps the global
 *            index n≥2 to the corresponding row r and column k and *
 *            evaluate...
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 37, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Helper: find the row r that contains index m (where m = n‑2) */
static idx_t
row_from_m(idx_t m)
{
    idx_t low = 2;
    idx_t high = m + 2;            /* enough to be above the answer */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 val = (unsigned __int128)mid * (mid - 1) / 2;
        if (val > m)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Helper: compute the coefficient T(n,k) of the polynomial for given row n
 * and column k (0‑based from constant term). */
static seq_t
T_row_k(idx_t n, idx_t k)
{
    seq_t pos = 0;          /* sum of terms with even j */
    seq_t neg = 0;          /* sum of terms with odd j  */
    seq_t pow = 1;          /* n^j  */
    idx_t limit = n - 1 - k;

    for (idx_t j = 0; j <= limit; ++j) {
        seq_t binom_nj   = binomial(n, j);
        seq_t binom_rest = binomial(n - 1 - j, k + 1);
        seq_t term = pow;
        term *= binom_nj;
        term *= binom_rest;

        if ((j & 1) == 0)
            pos += term;
        else
            neg += term;

        pow *= n;
    }

    /* The overall sign is (−1)^{k+n}.  Since the final value is always
     * positive, we return the absolute difference of the two partial
     * sums.  This is safe because |pos−neg| equals the desired result. */
    return (pos > neg) ? (pos - neg) : (neg - pos);
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* offset is 2; behavior below irrelevant */

    idx_t m = n - 2;               /* m counts entries from the start of the triangle */
    idx_t r = row_from_m(m);       /* determine the row */

    unsigned __int128 cumul = (unsigned __int128)(r - 2) * (r - 1) / 2;
    idx_t k = m - (idx_t)cumul;   /* column within the row (constant‑term index) */

    return T_row_k(r, k);
}

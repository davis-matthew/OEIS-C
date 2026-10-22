/*
 * A371764  Triangle read by rows: Trace of the Akiyama-Tanigawa algorithm for
 *          powers x^3.
 * https://oeis.org/A371764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where * r = floor((sqrt(8n + 1) - 1) / 2), k = n
 *            - r(r+1)/2, * T(n, k) = n^3 if n == k * = 2*C(k+3, k)*3^(n-k+1)
 *            * - 6*C(k+2, k)*2^(n-k+1) * + 7*(k + 1) otherwise
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, floor_sqrt, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute row index r = floor((sqrt(8n+1)-1)/2) */
    idx_t tmp = 8 * n + 1;
    seq_t sqrt_tmp = floor_sqrt((seq_t)tmp);          /* sqrt_tmp fits in seq_t */
    idx_t r = ((idx_t)sqrt_tmp - 1) / 2;

    /* Column index within the row */
    idx_t first_in_row = r * (r + 1) / 2;
    idx_t k = n - first_in_row;

    /* Diagonal entry: n^3 */
    if (r == k) {
        seq_t rt = (seq_t)r;
        return rt * rt * rt;
    }

    /* Off‑diagonal entry */
    idx_t exp = r - k + 1;          /* exp >= 1 because r > k here */

    seq_t bin1 = binomial(k + 3, k);   /* C(k+3, k) */
    seq_t bin2 = binomial(k + 2, k);   /* C(k+2, k) */
    seq_t pow3v = pow3(exp);           /* 3^exp */
    seq_t pow2v = pow2(exp);           /* 2^exp */

    seq_t term1 = 2 * bin1 * pow3v;    /* 2 * C(k+3, k) * 3^exp */
    seq_t term2 = 6 * bin2 * pow2v;    /* 6 * C(k+2, k) * 2^exp */
    seq_t result = term1 - term2 + 7 * ((seq_t)k + 1);

    return result;
}

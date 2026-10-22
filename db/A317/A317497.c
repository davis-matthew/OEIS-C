/*
 * A317497  Triangle T(n,k) = 3*T(n-1,k) + T(n-3,k-1) for k = 0..floor(n/3)
 *          with T(0,0) = 1 and T(n,k) = 0 for n or k < 0, read by rows.
 * https://oeis.org/A317497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) = T(r,k) where * r is the smallest non‑negative integer
 *            such that * Σ_{i=0}^{r} (⌊i/3⌋ + 1) > n * k = n – Σ_{i=0}^{r-1}
 *            (⌊i/3⌋ + 1) * T(r,k) = 3^(r‑3k) * (r‑2k)! / (k! · (r‑3k)!) * =
 *            3^(r‑3k) * binomial(r‑2k, k) (for r ≥ 3k) * * The triangle is
 *            read row‑by‑row; a(0)=1
 * Range:     u8 n <= 8, u16 n <= 25, u32 n <= 83, u64 n <= 300,
 *            u128 n <= 1107, big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t r = 0;        /* current row */
    idx_t cum = 0;      /* cumulative entries up to (but not including) row r */

    /* Find the row that contains the n-th term. */
    while (1) {
        idx_t row_len = (r / 3) + 1;           /* entries in row r */
        if (cum + row_len > n)                /* n lies in this row */
            break;
        cum += row_len;
        ++r;
    }

    /* Position of the term within the row. */
    idx_t offset = n - cum;
    idx_t k = offset;                         /* row index k */

    /* Compute the triangle entry T(r,k). */
    idx_t pow_exp = r - 3 * k;                /* exponent for 3 */
    seq_t pow_val = pow3(pow_exp);            /* 3^(r-3k) */
    seq_t bin_val = binomial(r - 2 * k, k);   /* (r-2k)! / (k! (r-3k)!) */

    return pow_val * bin_val;
}

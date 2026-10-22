/*
 * A363398  Triangle read by rows. T(n, k) = [x^k] P(n, x), where P(n, x) =
 *          Sum_{k=0..n} 2^(n - k) * Sum_{j=0..k} (x^j * binomial(k, j) * (2*j
 *          + 1)^n), (secant case).
 * https://oeis.org/A363398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A363398: Triangle read by rows.
   Formula: a(n) = T(r,k) where r = floor((√(8n+1)-1)/2),
   k = n - r(r+1)/2, and
   T(r,k) = (2k+1)^r * (2^(r+1) - ∑_{j=0}^k C(r+1,j)).
   The sequence begins 1,3,3,7,36,25,15,297,625,343,31,...
   All calculations are exact over seq_t.

   The implementation follows the explicit closed formula
   T(r,k) = (2k+1)^r * (2^(r+1) - Σ_{j=0}^k binom(r+1,j)).
   The helper 'big_pow' is used for exponentiation, while 'pow2'
   gives 2^(r+1).  Binomial coefficients are generated multiplicatively
   and summed directly.
*/
#include <stdint.h>
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */
#include "../utils/pow2.h"      /* pow2(idx_t exp) -> seq_t */

seq_t a(idx_t n)
{
    /* Determine the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start_of_row = r * (r + 1) / 2;
    idx_t k = n - start_of_row;          /* column index in row r */

    /* Compute (2k+1)^r using big_pow */
    seq_t base = (seq_t)(2 * k + 1);
    seq_t pow_base = big_pow(base, (seq_t)r);

    /* Compute 2^(r+1) */
    seq_t pow2_val = pow2((idx_t)r + 1);

    /* Sum_{j=0..k} binom(r+1, j) */
    seq_t sum = 0;
    seq_t bin = 1;                        /* binom(r+1,0) */
    sum += bin;
    for (idx_t j = 1; j <= k; ++j) {
        /* binom(r+1, j) = binom(r+1, j-1) * (r+2-j) / j */
        bin = bin * (seq_t)(r + 2 - j) / (seq_t)j;
        sum += bin;
    }

    /* Compute T(r,k) = pow_base * (pow2_val - sum) */
    seq_t factor = pow2_val - sum;
    return pow_base * factor;
}

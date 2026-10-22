/*
 * A329943  Square array read by antidiagonals: T(n,k) is the number of right
 *          total relations between set A with n elements and set B with k
 *          elements.
 * https://oeis.org/A329943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the (r,c) entry of the square array T(r,c) = (2^r - 1)^c
 *            read by antidiagonals
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Find the diagonal index k such that k*(k+1)/2 ≥ n  */
    idx_t k = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++k;
        sum += k;
    }

    /* Number of elements before this diagonal */
    idx_t prev = (k - 1) * k / 2;

    /* Position inside the diagonal (0‑based) */
    idx_t offset = n - prev - 1;

    /* Compute row r (i) and column c (j) in the array:
       r decreases from k to 1, c increases from 1 to k.       */
    idx_t r = k - offset;          /* 1 ≤ r ≤ k     */
    idx_t c = offset + 1;          /* 1 ≤ c ≤ k     */

    /* Compute (2^r - 1)^c */
    seq_t base = pow_uint(2, r);   /* 2^r   */
    base = base - 1;               /* 2^r-1 */
    seq_t exp = c;                 /* exponent */
    return big_pow(base, exp);
}

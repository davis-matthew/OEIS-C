/*
 * A356117  T(n, k) = [x^k] (1/2 - x)^(-n) - (1 - x)^(-n).
 * https://oeis.org/A356117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2^(i+j)-1) * C(i+j-1, j) * where i =
 *            floor((sqrt(8n+1)-1)/2), j = n - i*(i+1)/2, * and T(i,j) =
 *            (2^(i+j)-1) * C(i+j-1, j) (with T(0,0)=0)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2. */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n) {
        i++;
    }

    idx_t prev = i * (i + 1) / 2;   /* cumulative terms before row i */
    idx_t j   = n - prev;           /* position within row i */

    /* Handle the special first element T(0,0) = 0 explicitly. */
    if (i == 0 && j == 0) {
        return (seq_t)0;
    }

    /* Compute T(i,j) = (2^(i+j)-1) * C(i+j-1, j). */
    seq_t two_pow   = pow2(i + j);                     /* 2^(i+j) */
    seq_t factor    = two_pow - (seq_t)1;             /* 2^(i+j)-1   */
    seq_t binom     = binomial(i + j - 1, j);           /* C(i+j-1, j) */
    return factor * binom;
}

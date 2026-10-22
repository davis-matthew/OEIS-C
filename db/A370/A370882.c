/*
 * A370882  Square array T(n,k) = 9*2^k - n read by ascending antidiagonals.
 * https://oeis.org/A370882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A370882: Read the square array T(n,k) = 9*2^k - n by
 * ascending antidiagonals where, for each sum s = n+k, the pairs are
 * enumerated with n decreasing from s to 0. Here we compute the
 * corresponding value using only integer operations and the provided
 * pow2 function. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the antidiagonal index s such that
     *   s*(s+1)/2 <= n < (s+1)*(s+2)/2.
     * We use a simple loop; for n up to 10^4 this is trivial
     * (s ≲ 141). The loop also works for far larger n. */
    idx_t s = 0;
    while ( ((__int128)(s + 1) * (s + 2)) / 2 <= n )
        ++s;

    /* Number of elements before this antidiagonal */
    idx_t prev_sum = s * (s + 1) / 2;

    /* Position within the s-th antidiagonal */
    idx_t offset = n - prev_sum;

    /* For this position we have k = offset, n = s - offset */
    idx_t k = offset;
    idx_t n_in_array = s - offset;

    /* Compute the term: 9 * 2^k - n_in_array */
    seq_t pow2k = pow2(k);
    seq_t val = (seq_t)9 * pow2k - (seq_t)n_in_array;
    return val;
}

/*
 * A300401  Array T(n,k) = n*(binomial(k, 2) + 1) + k*(binomial(n, 2) + 1)
 *          read by antidiagonals.
 * https://oeis.org/A300401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the antidiagonal traversal of the
 *            array T(i,j) defined by T(i,j) = i * ( (j*(j-1))/2 + 1 ) + j * (
 *            (i*(i-1))/2 + 1 ). The pair (i,j) for a given index n is
 *            obtained by s = floor((sqrt(8*n+1)-1)/2) // diagonal index k = n
 *            - s*(s+1)/2 // position inside diagonal i = k j =...
 * Range:     u8 n <= 109, u16 n <= 3437, u32 n <= 9557, u64 n <= 9557,
 *            u128 n <= 9557, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <inttypes.h>

seq_t a(idx_t n) {
    /* Find diagonal index s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
    idx_t low = 0;
    idx_t high = 1;
    while (( (__uint128_t)high * (high + 1) / 2 ) <= n)
        high <<= 1;                 /* exponential search */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (( (__uint128_t)mid * (mid + 1) / 2 ) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t s = low - 1;

    /* Number of terms before diagonal s. */
    idx_t before = (idx_t)( ( (__uint128_t)s * (s + 1) ) / 2 );
    idx_t offset = n - before;            /* position inside diagonal */
    idx_t i = offset;
    idx_t j = s - offset;

    /* Convert indices to seq_t and compute T(i,j). */
    seq_t I = (seq_t)i;
    seq_t J = (seq_t)j;

    seq_t ibinom = 0;
    if ((seq_t)i >= 2) ibinom = I * (I - 1) / 2;
    seq_t jbinom = 0;
    if ((seq_t)j >= 2) jbinom = J * (J - 1) / 2;

    seq_t term1 = I * (jbinom + 1);
    seq_t term2 = J * (ibinom + 1);
    return term1 + term2;
}

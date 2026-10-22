/*
 * A329840  Beatty sequence for (9+sqrt(41))/4.
 * https://oeis.org/A329840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(9+sqrt(41))/4)
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 4845, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n is the index, 1‑based.  Compute floor(n * (9+sqrt(41))/4)
       by using the identity  floor(n*sqrt(41)) = floor( sqrt(41*n*n) ). */
    seq_t n_seq = (seq_t)n;

    seq_t n_sq = n_seq * n_seq;          /* n^2  */
    seq_t prod = (seq_t)41 * n_sq;        /* 41*n^2  */

    seq_t sqrt_val = floor_sqrt(prod);   /* floor(n*sqrt(41))  */

    seq_t sum = (seq_t)9 * n_seq + sqrt_val;  /* 9*n + floor(n*sqrt(41)) */
    return sum / 4;                               /* floor((9n+floor(n*sqrt(41)))/4) */
}

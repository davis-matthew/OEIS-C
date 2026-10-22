/*
 * A329835  Beatty sequence for (9+sqrt(101))/10.
 * https://oeis.org/A329835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (9*n + floor(n*sqrt(101)) ) / 10 )
 * Range:     u8 none, u16 n <= 12, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute floor(n*sqrt(101)) as floor(sqrt(101*n^2)) */
    seq_t n_seq   = (seq_t)n;
    seq_t nsq     = n_seq * n_seq;             /* n^2    */
    seq_t prod    = nsq * (seq_t)101;          /* 101*n^2 */
    seq_t sqrtv   = floor_sqrt(prod);          /* floor(sqrt(101*n^2)) = floor(n*sqrt(101)) */

    seq_t nine_n  = (seq_t)9 * n_seq;
    seq_t numer   = nine_n + sqrtv;            /* 9*n + floor(n*sqrt(101)) */
    seq_t result  = numer / (seq_t)10;         /* floor((9*n + floor(n*sqrt(101))) / 10) */

    return result;
}

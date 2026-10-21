/*
 * A286355  Beatty sequence of (10 + sqrt(10))/9; complement of A177102.
 * https://oeis.org/A286355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(10 + sqrt(10))/9)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 1545, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* Compute floor(n*sqrt(10)) as floor(sqrt(10*n^2)) */
    seq_t nn   = (seq_t)n;
    seq_t prod = nn * nn;          /* n^2 */
    prod = prod * (seq_t)10;        /* 10*n^2 */
    seq_t s = isqrt_seq(prod);      /* floor(n*sqrt(10)) */

    /* Compute floor((n*10 + floor(n*sqrt(10))) / 9) */
    seq_t sum = nn * (seq_t)10 + s;
    return sum / (seq_t)9;
}

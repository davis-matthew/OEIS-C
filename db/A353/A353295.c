/*
 * A353295  Square nearest to the sum of the first n positive squares.
 * https://oeis.org/A353295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest square to n(n+1)(2n+1)/6
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 2342, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t sum = n_seq * (n_seq + 1) * (2 * n_seq + 1) / 6;

    /* nearest square */
    seq_t sq = floor_sqrt(sum);          /* floor(sqrt(sum))          */
    seq_t sq2 = sq * sq;                 /* sq^2                      */
    seq_t nxt = sq + 1;
    seq_t nxt2 = nxt * nxt;              /* (sq+1)^2                  */

    seq_t diff1 = sum - sq2;             /* sum - sq^2                */
    seq_t diff2 = nxt2 - sum;            /* (sq+1)^2 - sum            */

    return (diff1 <= diff2) ? sq2 : nxt2;
}

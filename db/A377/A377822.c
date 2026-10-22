/*
 * A377822  a(n) is the least q>0 such that abs(p^2/q^2 - n)<=1/4 for a
 *          suitable integer p.
 * https://oeis.org/A377822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least q>0 such that |p^2/q^2 - n| <= 1/4 for some integer
 *            p
 * Range:     u8 n <= 4, u16 n <= 79, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* Perfect squares return 1 immediately. */
    seq_t sq = floor_sqrt(nn);
    if (sq * sq == nn) {
        return (seq_t)1;
    }

    for (idx_t q = 2; ; ++q) {
        seq_t qq = (seq_t)q;
        seq_t prod = nn * (qq * qq);        /* prod = n * q^2 */
        seq_t sqrtp = floor_sqrt(prod);     /* floor(sqrt(prod)) */

        /* Check p = floor(sqrt(prod)) */
        seq_t p0 = sqrtp;
        seq_t p0sq = p0 * p0;
        seq_t diff0 = (p0sq >= prod) ? (p0sq - prod) : (prod - p0sq);
        seq_t q2 = qq * qq;
        if ((diff0 << 2) <= q2) {
            return qq;
        }

        /* Check p = floor(sqrt(prod)) + 1 */
        seq_t p1 = p0 + 1;
        seq_t p1sq = p1 * p1;
        seq_t diff1 = (p1sq >= prod) ? (p1sq - prod) : (prod - p1sq);
        if ((diff1 << 2) <= q2) {
            return qq;
        }
    }
}

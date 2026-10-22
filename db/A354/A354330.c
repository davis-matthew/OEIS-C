/*
 * A354330  Distance from the sum of the first n positive triangular numbers
 *          to the nearest triangular number.
 * https://oeis.org/A354330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = distance between the tetrahedral number T(n) =
 *            n*(n+1)*(n+2)/6 and the nearest triangular number
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 681, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* triangular number k(k+1)/2, computed safely */
static seq_t triangular(seq_t k)
{
    seq_t a = k;
    seq_t b = k + 1;
    if (a % 2 == 0)
        a /= 2;
    else
        b /= 2;
    return a * b;   /* a * b = k(k+1)/2             */
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t a1 = nn;
    seq_t b1 = nn + 1;
    seq_t c1 = nn + 2;

    /* compute t = n*(n+1)*(n+2)/6 safely              */
    /* divide by 2 */
    if (a1 % 2 == 0)
        a1 /= 2;
    else if (b1 % 2 == 0)
        b1 /= 2;
    else
        c1 /= 2;

    /* divide by 3 */
    if (a1 % 3 == 0)
        a1 /= 3;
    else if (b1 % 3 == 0)
        b1 /= 3;
    else
        c1 /= 3;

    seq_t t = a1 * b1;
    t = t * c1;   /* t = n*(n+1)*(n+2)/6 */

    /* nearest triangular number to t               */
    seq_t m = t * 8 + 1;
    seq_t s = floor_sqrt(m);        /* floor(sqrt(8t+1))        */

    seq_t k1 = (s - 1) / 2;
    seq_t T1 = triangular(k1);
    seq_t diff1 = (t > T1) ? (t - T1) : (T1 - t);

    seq_t k2 = k1 + 1;
    seq_t T2 = triangular(k2);
    seq_t diff2 = (t > T2) ? (t - T2) : (T2 - t);

    return (diff1 < diff2) ? diff1 : diff2;
}

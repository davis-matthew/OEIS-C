/*
 * A379777  Array A(n, k), n, k >= 0, read by upward antidiagonals; for any v
 *          >= 0, the value appears twice in the array: in row A002262(v) and
 *          in row A002024(v+1); values in each row are given in strictly
 *          increasing order.
 * https://oeis.org/A379777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   compute a(n) by reading the array A(n,k) along upward
 *            antidiagonals
 * Range:     u8 n <= 298, u16 n <= 9339, u32 n <= 9339, u64 n <= 9339,
 *            u128 n <= 9339, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A(n,k) is defined by: if n <= k  -> A(n,k) = T(k) + n,
   where T(k)=k*(k+1)/2;  otherwise apply 
   A(n,k) = A(k+1,n) and repeat until the first index <= second index. */

#include <stdint.h>
#include <math.h>

/* compute a(n).  idx_t and seq_t are provided by the build. */
seq_t a(idx_t n)
{
    /* ---------- find diagonal index t ----------
       Let t be the largest integer such that t*(t+1)/2 <= n.
       Use long double sqrt to get an approximation then adjust.  */
    uint64_t t = (uint64_t)((sqrt((long double)(8.0L * n + 1.0L)) - 1.0L) / 2.0L);

    /* adjust downwards if overshoot */
    while ((unsigned __int128)t * (t + 1) / 2 > n) --t;
    /* adjust upwards if possible */
    while ((unsigned __int128)(t + 1) * (t + 2) / 2 <= n) ++t;

    /* position p in this diagonal, 0 <= p <= t */
    uint64_t prev = (unsigned __int128)t * (t + 1) / 2;
    uint64_t p = n - prev;

    /* coordinates (i,j) on the antidiagonal */
    uint64_t i = t - p;
    uint64_t j = p;

    /* transform pair until first <= second */
    while (i > j) {
        uint64_t tmp = j;
        j = i - 1;
        i = tmp;
    }

    /* final value: T(j) + i = j*(j+1)/2 + i */
    seq_t val = (seq_t)j * (j + 1) / 2 + (seq_t)i;
    return val;
}

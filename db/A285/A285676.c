/*
 * A285676  Beatty sequence of (7 + sqrt(7))/6; complement of A022841.
 * https://oeis.org/A285676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(7+sqrt(7))/6) = n + floor(n*(1+sqrt(7))/6)
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Computes a(n) for the Beatty sequence of (7+sqrt(7))/6. */
seq_t a(idx_t n)
{
    /* n >= 1  */
    /* ---------- compute the integer part of n/6 ---------- */
    idx_t qN = n / 6;
    unsigned rN = n % 6;

    /* ---------- compute floor(sqrt(7*n*n)) ---------- */
    seq_t tmp = (seq_t)n;   /* cast to seq_t to avoid overflow */
    tmp = tmp * tmp;        /* n^2  */
    tmp = tmp * 7;          /* 7*n^2 */
    seq_t sqrtS = floor_sqrt(tmp);

    /* ---------- split sqrtS into quotient and remainder by 6 ---------- */
    seq_t qS = sqrtS / 6;
    unsigned rS = (unsigned)(sqrtS % 6);

    /* --------- floor(n*(1+sqrt(7))/6) = qN + qS (+carry) --------- */
    seq_t floor_nbeta = (seq_t)qN + qS;
    if (rN + rS >= 6) {
        floor_nbeta += 1;
    }

    /* ---------- final result a(n) = n + floor_nbeta ---------- */
    return (seq_t)n + floor_nbeta;
}

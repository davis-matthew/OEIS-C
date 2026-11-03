/*
 * A285676  Beatty sequence of (7 + sqrt(7))/6; complement of A022841.
 * https://oeis.org/A285676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(7+sqrt(7))/6) = n + floor(n*(1+sqrt(7))/6)
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 8485 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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

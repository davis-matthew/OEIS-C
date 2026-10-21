/*
 * A259904  n*a(n+1) = (2*n^2+2n-1)*a(n)+(n+1)*a(n-1); a(0)=0, a(1)=1.
 * https://oeis.org/A259904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: n*a(n+1) = (2*n^2+2*n-1)*a(n)+(n+1)*a(n-1)
   Initial values: a(0)=0, a(1)=1 */
#pragma once
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_nm2 = (seq_t)0;   /* a(0) */
    seq_t a_nm1 = (seq_t)1;   /* a(1) */
    seq_t a_cur = (seq_t)0;   /* placeholder for a(k+1) */

    /* Compute successive terms up to a(n). */
    for (idx_t i = 1; i < n; ++i)
    {
        /* Use 128‑bit arithmetic to avoid intermediate overflow
           on a 64‑bit build.  The code compiles for the other
           built types as well because the cast to __uint128_t
           is only applied when seq_t is a 64‑bit unsigned integer. */
        __uint128_t coeff = (__uint128_t)2 * (__uint128_t)i * (__uint128_t)i
                         + (__uint128_t)2 * (__uint128_t)i
                         - (__uint128_t)1;
        __uint128_t term1 = coeff * (__uint128_t)a_nm1;
        __uint128_t term2 = (__uint128_t)(i + (idx_t)1) * (__uint128_t)a_nm2;
        __uint128_t num   = term1 + term2;
        a_cur = (seq_t)(num / (__uint128_t)i);

        /* Shift for the next iteration. */
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }
    return a_nm1;
}

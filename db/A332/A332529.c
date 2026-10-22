/*
 * A332529  Rectangular array by antidiagonals: T(n,k) = floor(n + k*r), where
 *          r = golden ratio = (1+sqrt(5))/2.
 * https://oeis.org/A332529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function implements OEIS A332529:
 * T(n,k) = floor(n + k*r) where r = φ² = (3+√5)/2.
 * The sequence a(n) is obtained by listing the array T(n,k) along
 * antidiagonals: a(0)=T(0,0), a(1)=T(0,1), a(2)=T(1,0), …
 * The routine uses integer arithmetic only; the irrational r appears
 * only via the Beatty sequence floor(k*φ) which is evaluated with a
 * rational approximation of φ to avoid floating‑point operations.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root of a 128‑bit unsigned value. */
static inline idx_t isqrt_u128(unsigned __int128 v)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64) - 1;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid <= v) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return (idx_t)high;
}

seq_t a(idx_t n)
{
    /* 8*n + 1 fits in 128 bits; use __int128 to avoid overflow. */
    unsigned __int128 v = (unsigned __int128)8 * n + 1;
    idx_t d = (isqrt_u128(v) - 1) / 2;          /* diagonal index */

    /* previous diagonal length: d*(d+1)/2 */
    unsigned __int128 prev128 = (unsigned __int128)d * (d + 1) / 2;
    idx_t   n0 = n - (idx_t)prev128;            /* horizontal offset */
    idx_t   i  = n0;                           /* row index */
    idx_t   j  = d - n0;                       /* column index */

    /* floor(j * φ) = floor(j * 832040 / 514229)   (high‑precision rational) */
    seq_t  J = (seq_t)j;
    seq_t  floor_j_phi = (J * (seq_t)832040) / (seq_t)514229;

    /* floor(j * r) = j + floor(j*φ)   where r = φ + 1 */
    seq_t  floor_j_r = (seq_t)j + floor_j_phi;

    return (seq_t)i + floor_j_r;
}

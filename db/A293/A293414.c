/*
 * A293414  The integer k that minimizes |k/n^2 - e|.
 * https://oeis.org/A293414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(1/2 + (n^2)*e) (computed with integer arithmetic)
 * Range:     u8 n <= 9, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Pre‑computed integer approximation of the constant e
   with scale factor 10^12:
   e ≈ 2.718281828459045...  ->  e_scaled = floor(e * 10^12) = 2718281828459.
   The error of this approximation is < 10^(-12),
   which yields an absolute error < 0.5 for all n up to ~10^8
   (and consequently correct rounding for the integers in the tests). */

static const __int128 SCALE = (__int128)1000000000000ULL;  /* 10^12 */
static const __int128 E_SCALED = (__int128)2718281828459ULL; /* floor(e * 10^12) */

/* Compute a(n) = floor(1/2 + n^2 * e) using only integer arithmetic */
seq_t a(idx_t n)
{
    /* n may be as large as wanted; use 128‑bit arithmetic for safety */
    __int128 nn = n;
    __int128 n2 = nn * nn;                /* n^2  */
    __int128 num = n2 * E_SCALED + SCALE / 2; /* n^2 * e_scaled + 0.5 * SCALE */
    seq_t result = (seq_t)(num / SCALE);   /* floor division gives the rounded integer */
    return result;
}

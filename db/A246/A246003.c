/*
 * A246003  Floor(m^n/n) with n >= m >= 1.
 * https://oeis.org/A246003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(m^n / n) where the triangle row r contains values
 *            for m = 1 .. r, and a(n) enumerates this triangle in row‑major
 *            order. Position: a(1) = 1; a(2) = 0; a(3) = 2; a(4) = 0; ...
 *            (See OEIS A246003)
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Find the row r such that T(r‑1) < n ≤ T(r)  (T(k) = k(k+1)/2). */
    idx_t r = 1;
    while ( (r*(r+1)/2) < n )
        ++r;

    /* Offset within the row: m = n – T(r‑1). */
    idx_t prev = (r-1)*r/2;   /* elements before row r */
    idx_t m    = n - prev;    /* 1 ≤ m ≤ r */

    /* Compute floor(m^r / r) using integer arithmetic. */
    seq_t pow = 1;
    seq_t seq_m = (seq_t)m;
    for (idx_t i = 0; i < r; ++i)
        pow *= seq_m;              /* m^r  (exact when fits in seq_t) */

    seq_t denom = (seq_t)r;
    return pow / denom;            /* integer division gives floor */
}

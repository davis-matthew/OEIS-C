/*
 * A365582  a(n) is the number at the third vertex of an equilateral triangle
 *          whose first and second vertices are at the numbers 1 and n,
 *          respectively, on a triangular array of integers.
 * https://oeis.org/A365582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (m^4 - 2m^3 + (-4n-1)m^2 + (4n-6)m + 4n^2 + 4n + 8) / 8
 *            where m = floor((sqrt(8n-7)+1)/2)
 * Range:     i8 n <= 20, i16 n <= 7130, i32 n <= 9626, i64 n <= 9626,
 *            i128 n <= 9626, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(x)) for 0 <= x <= ULLONG_MAX using binary search. */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t low = 0, high = ((idx_t)1 << 32);  /* 2^32 > sqrt(ULLONG_MAX) */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* m = floor((sqrt(8*n-7)+1)/2) */
    idx_t t = 8 * n - 7;                    /* 8*n-7  (fits in uint64_t for typical n) */
    idx_t s = floor_sqrt_u64(t);            /* floor(sqrt(8*n-7)) */
    idx_t m_raw = (s + 1) / 2;              /* integer division floors automatically */

    /* All calculations in seq_t to preserve sign and avoid overflow */
    seq_t nn = (seq_t)n;
    seq_t mm = (seq_t)m_raw;

    /* Compute numerator: m^4 - 2m^3 + (-4n-1)m^2 + (4n-6)m + 4n^2 + 4n + 8 */
    seq_t m_sq  = mm * mm;            /* m^2  */
    seq_t m_cu  = m_sq * mm;          /* m^3  */
    seq_t m_qu  = m_cu * mm;          /* m^4  */

    seq_t numerator = 0;
    numerator += m_qu;                                 /* + m^4          */
    numerator -= (seq_t)2 * m_cu;                       /* - 2 m^3        */
    numerator += ( (- (seq_t)4 * nn) - 1 ) * m_sq;      /* + (-4n-1) m^2  */
    numerator += ( (seq_t)4 * nn - 6 ) * mm;            /* + (4n-6) m     */
    numerator += (seq_t)4 * nn * nn;                    /* + 4 n^2        */
    numerator += (seq_t)4 * nn;                         /* + 4 n          */
    numerator += (seq_t)8;                              /* + 8            */

    /* Divide by 8 (exact division) */
    return numerator / 8;
}

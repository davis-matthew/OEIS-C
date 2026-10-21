/*
 * A286989  Positions of 1 in A286987; complement of A286988.
 * https://oeis.org/A286989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)*(10+sqrt(2))/7) + 1 (Beatty sequence with α =
 *            (10+sqrt(2))/7)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 5747, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for arbitrary‑precision seq_t */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low  = 0;
    seq_t high = x;
    while (low <= high)
    {
        seq_t mid = (low + high) >> 1;
        seq_t sq  = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* safety for offset 1 */
        return 0;

    idx_t m = n - 1;                     /* m = n-1  (m >= 0)           */
    seq_t mm  = (seq_t)m;                /* m as seq_t                     */
    seq_t two = 2;

    /* t = 2 * m * m  ( = (m * sqrt(2))^2 ) */
    seq_t t = two * mm * mm;

    /* y0 = floor(sqrt(t)) = floor(m * sqrt(2))              */
    seq_t y0 = isqrt_seq(t);

    /* Compute r such that y ≡ r (mod 7) with r = (-10*m) mod 7 */
    uint64_t rem10m = (10ULL * (uint64_t)m) % 7ULL;
    unsigned int r = (7 - (unsigned int)rem10m) % 7;   /* r in 0..6 */
    unsigned int y0_mod = (unsigned int)(y0 % 7);

    /* Largest y <= y0 with y ≡ r (mod 7) */
    unsigned int diff = (y0_mod + 7 - r) % 7;  /* diff <= 6       */
    seq_t y = y0 - (seq_t)diff;

    /* k = floor((10*m + m*sqrt(2)) / 7) = (y + 10*m) / 7   */
    seq_t ten_m = mm * 10;
    seq_t numerator = y + ten_m;
    seq_t k = numerator / 7;

    return k + 1;
}

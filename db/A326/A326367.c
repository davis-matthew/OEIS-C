/*
 * A326367  Number of tilings of an equilateral triangle of side length n with
 *          unit triangles (of side length 1) and exactly two unit "lozenges"
 *          or "diamonds" (also of side length 1).
 * https://oeis.org/A326367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/8)*(n-2)*(n-1)*(3*n^2 + 3*n - 4) (valid for n >= 1)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 249, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 2) return 0;

    /* Use 64‑bit unsigned integers for intermediate calculations;
       if the compiler supports 128‑bit, use that to avoid overflow. */
    uint64_t ns   = (uint64_t)n;
    uint64_t a    = ns - 2;
    uint64_t b    = ns - 1;
    uint64_t poly = 3*ns*ns + 3*ns - 4;

#ifdef __SIZEOF_INT128__
    unsigned __int128 num = (unsigned __int128)3 * a * b * poly;
    seq_t result = (seq_t)(num / 8);
#else
    /* 64‑bit intermediate is sufficient for n up to ~10000 (a(n) < 1e16) */
    uint64_t num = 3 * a * b * poly;
    seq_t result = (seq_t)(num / 8);
#endif

    return result;
}

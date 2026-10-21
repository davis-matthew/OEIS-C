/*
 * A251607  Number of non-congruent integer triangles ABC with largest side BC
 *          of length n whose opposite vertex A lies inside the closed
 *          boundary formed by BC and a locus of points whose real distance
 *          from B is x, from C is y and x^3 + y^3 = n^3.
 * https://oeis.org/A251607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered integer pairs (x,y) with 1 ≤ x ≤ y ≤
 *            n, x + y > n, and x³ + y³ ≤ n³
 * Range:     u8 n <= 37, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count such pairs for a given n. */
#include <stdint.h>
#include <stdbool.h>

/* Integer cube root floor: largest r with r³ ≤ x.  Works for x ≤ 2⁶⁴ – 1. */
static inline unsigned long long cube_root_floor(unsigned long long x)
{
    unsigned long long lo = 0, hi = 0xFFFFFFFFULL; /* 2^32 > 10¹², enough for n≤10⁴ */
    unsigned long long res = 0;
    while (lo <= hi) {
        unsigned long long mid = (lo + hi) >> 1;
        __uint128_t m3 = (__uint128_t)mid * mid * mid;
        if (m3 <= x) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Treat n as unsigned 64‑bit to avoid signed underflow in subtraction. */
    unsigned long long un = (unsigned long long)n;
    unsigned long long n3 = un * un * un;           /* n³, safe for n ≤ 10⁴ */
    seq_t result = 0;
    for (idx_t ix = 1; ix <= n; ++ix) {
        unsigned long long x = (unsigned long long)ix;
        unsigned long long x3 = x * x * x;

        unsigned long long diff = n3 - x3;           /* n³ – x³ ≥ 0   */
        unsigned long long y_max = cube_root_floor(diff);
        if (y_max > un) y_max = un;                 /* limit by y ≤ n      */

        unsigned long long y_min = ix;              /* y ≥ x            */
        unsigned long long tmp = un - x + 1;        /* y > n – x → ≥ n – x + 1 */
        if (tmp > y_min) y_min = tmp;               /* y ≥ max(x, n–x+1)  */

        if (y_max >= y_min) {
            /* Count of integers in [y_min, y_max] */
            result += (seq_t)(y_max - y_min + 1);
        }
    }
    return result;
}

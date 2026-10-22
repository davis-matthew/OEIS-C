/*
 * A375873  A family of triangles T(m), m >= 0, read by triangles and then by
 *          rows; let P(m) be the triangle made up of the first m+1 rows of
 *          Pascal's triangle (A007318), R(m) (respectively L(m)) be P(m)
 *          rotated to the right (respectively to the left), T(m)(n, k) =
 *          gcd(P(m)(n, k), R(m)(n, k), L(m)(n, k)).
 * https://oeis.org/A375873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = read off the triangles T(m) by triangles, then by rows.
 *            T(m)(n, k) = gcd(binomial(n, k), binomial(m - k, n - k),
 *            binomial(m - n + k, m - n)). Here the sequence a(n) lists all
 *            entries of all T(0), T(1), T(2), … in that order
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/binomial.h"

/* Tetrahedral number S(m) = (m+1)(m+2)(m+3)/6.  Returns value as unsigned __int128. */
static inline unsigned __int128 tetrahedral(idx_t m)
{
    return (unsigned __int128)(m + 1) * (m + 2) * (m + 3) / 6;
}

/* Triangular number T(r) = (r+1)(r+2)/2.  Returns value as unsigned __int128. */
static inline unsigned __int128 triangular(idx_t r)
{
    return (unsigned __int128)(r + 1) * (r + 2) / 2;
}

/* Find minimal m such that tetrahedral(m) > n.  Precondition: n >= 0. */
static inline idx_t find_m(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Grow high until it exceeds n. */
    while (tetrahedral(high) <= (unsigned __int128)n) {
        high <<= 1;
    }
    /* Binary search for m. */
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if (tetrahedral(mid) <= (unsigned __int128)n) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return high; /* minimal m with S(m) > n */
}

seq_t a(idx_t n)
{
    /* Step 1: locate which triangle m contains index n. */
    idx_t m = find_m(n);

    /* Step 2: compute offset of the previous triangle. */
    unsigned __int128 prev_offset128 = 0;
    if (m > 0) {
        prev_offset128 = (unsigned __int128)m * (m + 1) * (m + 2) / 6; /* S(m-1) */
    }
    idx_t pos = n - (idx_t)prev_offset128; /* position inside triangle m [0 .. S(m)-S(m-1)-1] */

    /* Step 3: locate row r within the triangle: minimal r with triangular(r) > pos. */
    idx_t low = 0, high = m;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (triangular(mid) <= (unsigned __int128)pos) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    idx_t r = low; /* row index (0-based) */

    /* Step 4: column k within row r. */
    unsigned __int128 row_start128 = (unsigned __int128)r * (r + 1) / 2; /* elements before this row */
    idx_t k = pos - (idx_t)row_start128;

    /* Step 5: compute the three binomial coefficients. */
    seq_t c1 = binomial(r, k);
    seq_t c2 = binomial(m - k, r - k);
    seq_t c3 = binomial(m - r + k, m - r);

    /* Step 6: return gcd of the three values. */
    seq_t g = gcd(c1, c2);
    g = gcd(g, c3);
    return g;
}

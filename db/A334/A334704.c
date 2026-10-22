/*
 * A334704  Triangle read by rows: T(n,k) (1 <= k <= n) = number of ways to
 *          choose three collinear points from an n X k grid of points.
 * https://oeis.org/A334704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) counts the unordered triples of
 *            collinear lattice points in an r×k grid. r and k are derived
 *            from n by locating the triangle position: n lies in row r and
 *            column k, where n = 1 + 2 + … + r – (r‑1)(r‑2)/2 and 1 ≤ k ≤ r
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* small gcd for unsigned integers */
static unsigned gcd_uint(unsigned a, unsigned b)
{
    while (b) {
        unsigned t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* --- find the triple coordinates (r,k) ----------------------- */
    idx_t r = 1;
    idx_t sum = 1;
    while (sum < n) {
        ++r;
        sum += r;
    }
    /* previous triangular sum is (r-1) * r / 2 */
    idx_t k = n - (r - 1) * r / 2;

    /* --- vertical and horizontal lines -------------------------- */
    seq_t total = 0;

    if (r >= 3) {
        uint64_t cn = (uint64_t)r * (r - 1) * (r - 2) / 6;   /* C(r,3) */
        total += (seq_t)k * (seq_t)cn;                       /* k columns */
    }
    if (k >= 3) {
        uint64_t ck = (uint64_t)k * (k - 1) * (k - 2) / 6;   /* C(k,3) */
        total += (seq_t)r * (seq_t)ck;                       /* r rows */
    }

    /* --- diagonals with slope ≠ 0,∞ ---------------------------- */
    uint64_t diag = 0;
    for (idx_t dx = 1; dx < r; ++dx) {
        for (idx_t dy = 1; dy < k; ++dy) {
            unsigned g = gcd_uint((unsigned)dx, (unsigned)dy);
            if (g <= 1) continue;       /* gcd‑1 would be 0 */
            uint64_t pairs = 2ULL * (r - dx) * (k - dy);
            diag += pairs * (g - 1);
        }
    }
    total += (seq_t)diag;
    return total;
}

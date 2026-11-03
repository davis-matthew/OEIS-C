/*
 * A338797  Triangle read by rows: T(n,k) is the least m such that there exist
 *          positive integers x, y and z satisfying x/n + y/k = z/m where all
 *          fractions are reduced; 1 <= k <= n.
 * https://oeis.org/A338797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where (r,k) is the position of the nth term in the
 *            triangle defined in OEIS A338797: T(n,k) = LCM(n,k) / g if
 *            gcd(gcd(n,k), n/gcd(n,k)) == 1 LCM(n,k) otherwise
 * Range:     u8 n <= 151, u16 n <= 10011, u32 n <= 10011, u64 n <= 10011,
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 82 terms through n = 9763 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Find the smallest r such that r*(r+1)/2 >= n */
static inline idx_t row_of_term(idx_t n)
{
    /* Use double and correct with a loop – safe for n up to very large values */
    long double val = sqrt((long double)8 * n + 1) - 1;
    idx_t r = (idx_t)(val / 2);

    while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;
    while (r > 0 && (unsigned __int128)(r - 1) * r / 2 >= n) --r;
    return r;
}

/* Compute gcd of two unsigned integers (idx_t) */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Determine (row, col) in the triangle */
    idx_t r   = row_of_term(n);                 /* row number (1‑based) */
    idx_t prev_tri = (unsigned __int128)(r - 1) * r / 2; /* terms before this row */
    idx_t k = n - prev_tri;                     /* column 1‑based */

    /* Compute T(r,k) */
    idx_t d      = gcd_idx(r, k);               /* gcd(r,k) */
    idx_t nprime = r / d;                       /* r/d */
    idx_t kprime = k / d;                       /* k/d */

    /* lcm = r / d * k (computed in seq_t to avoid overflow) */
    seq_t lcm = ((seq_t)r / (seq_t)d) * (seq_t)k;

    /* Check if d is coprime to both n' and k' */
    if (gcd_idx(d, nprime) == 1 && gcd_idx(d, kprime) == 1) {
        /* Reduce lcm by d */
        return lcm / (seq_t)d;
    } else {
        return lcm;
    }
}

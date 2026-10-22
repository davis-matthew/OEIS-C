/*
 * A393016  Triangle read by rows: T(n, k) = numerator(8*n^3*k/(k^2+4*n^2)).
 * https://oeis.org/A393016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac, easy
 * Formula:   a(n) = numerator of 8*n^3*k/(k^2+4*n^2) read row‑wise where n is
 *            the 1‑based index in the triangle and 1 ≤ k ≤ n
 * Range:     u8 n <= 4, u16 n <= 53, u32 n <= 3240, u64 n <= 3240,
 *            u128 n <= 3240, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* gcd routine is provided by the build. */
#include "../utils/gcd.h"

/* Compute the nth term (1‑based) of OEIS A393016. */
seq_t a(idx_t n)
{
    /* The sequence is undefined for n = 0; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Find the row r such that r*(r+1)/2 ≥ n. */
    idx_t r = 1;
    idx_t tri = 1;      /* current value of r*(r+1)/2 */
    while (tri < n) {
        ++r;
        tri += r;
    }

    /* k is the position within the row: k = n - (r-1)*r/2. */
    idx_t prev_tri = tri - r;   /* (r-1)*r/2 */
    idx_t k = n - prev_tri;

    /* All arithmetic is performed in seq_t. */
    seq_t r_seq   = (seq_t)r;
    seq_t k_seq   = (seq_t)k;
    seq_t r_sq    = r_seq * r_seq;
    seq_t r_cube  = r_sq * r_seq;

    seq_t num = (seq_t)8;
    num *= r_cube;
    num *= k_seq;

    seq_t den = k_seq * k_seq + (seq_t)4 * r_sq;

    seq_t g = gcd(num, den);
    return num / g;
}

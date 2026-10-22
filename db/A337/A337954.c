/*
 * A337954  Number of chiral pairs of colorings of the 16 tetrahedral facets
 *          of a hyperoctahedron or of the 16 vertices of a tesseract.
 * https://oeis.org/A337954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) * n^2 * (n+1) * (n^12 + n^10 - 11*n^8 + n^6 +
 *            44*n^4 - 4*n^2 - 48) / 384
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 23, u128 n <= 371,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* convert n once to seq_t */
    seq_t n_s = (seq_t)n;

    /* powers of n */
    seq_t n2  = n_s * n_s;          /* n^2 */
    seq_t n4  = n2 * n2;            /* n^4 */
    seq_t n6  = n4 * n2;            /* n^6 */
    seq_t n8  = n4 * n4;            /* n^8 */
    seq_t n10 = n8 * n2;            /* n^10 */
    seq_t n12 = n8 * n4;            /* n^12 */

    /* polynomial part – all terms are positive for n >= 2 */
    seq_t poly = n12;
    poly += n10;
    poly -= 11 * n8;
    poly += n6;
    poly += 44 * n4;
    poly -= 4 * n2;
    poly -= 48;

    /* We compute (n-1) * n^2 * (n+1) * poly / 384,
       reducing the denominator 384 as we go to keep the
       intermediate product within the bit‑width of seq_t. */
    seq_t den  = 384;    /* denominator 2^7 * 3   */
    seq_t res  = 1;      /* accumulated product */

    /* multiply by the four factors while cancelling common
       factors with the current denominator */
    seq_t f = n_s - 1;   /* factor (n-1) */
    seq_t g = gcd(f, den);
    if (g != 0) { f /= g; den /= g; }
    res *= f;

    f = n2;              /* factor n^2 */
    g = gcd(f, den);
    if (g != 0) { f /= g; den /= g; }
    res *= f;

    f = n_s + 1;         /* factor (n+1) */
    g = gcd(f, den);
    if (g != 0) { f /= g; den /= g; }
    res *= f;

    f = poly;            /* factor poly */
    g = gcd(f, den);
    if (g != 0) { f /= g; den /= g; }
    res *= f;

    /* any remaining part of the denominator – should be 1 */
    if (den != 1)
        res /= den;

    return res;
}

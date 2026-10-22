/*
 * A330197  Number of scalene triangles whose vertices are the vertices of a
 *          regular n-gon.
 * https://oeis.org/A330197
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,3) - n*(n-1)/2 if n mod 6 = 1 or 5 a(n) = C(n,3) -
 *            n*(n-2)/2 if n mod 6 = 2 or 4 a(n) = C(n,3) - n*(3*n-7)/6 if n
 *            mod 6 = 3 a(n) = C(n,3) - n*(3*n-10)/6 if n mod 6 = 0 where
 *            C(n,3) = n*(n-1)*(n-2)/6. The function is valid for n >= 3
 * Range:     u8 n <= 13, u16 n <= 75, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    seq_t ns = (seq_t)n;
    seq_t binom = ns * (ns - 1) * (ns - 2) / 6;

    idx_t mod = n % 6;
    seq_t sub;
    if (mod == 1 || mod == 5) {
        sub = ns * (ns - 1) / 2;
    } else if (mod == 2 || mod == 4) {
        sub = ns * (ns - 2) / 2;
    } else if (mod == 3) {
        sub = ns * ((seq_t)3 * ns - 7) / 6;
    } else {                /* mod == 0 */
        sub = ns * ((seq_t)3 * ns - 10) / 6;
    }

    return binom - sub;
}

/*
 * A279034  The sum of the necessary diagonal movements from each square unit
 *          of an n X n+1 rectangle to reach any of the corners of the
 *          rectangle.
 * https://oeis.org/A279034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (13*n^3 + 3*(3*(n mod 2)+1)*n^2 - (28-9*(n mod 2))*n -
 *            6*(n mod 4)) / 24
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 710, u64 n <= 998,
 *            u128 n <= 998, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;
    unsigned m = n & 1U;          /* n mod 2 */
    unsigned r = n & 3U;          /* n mod 4 */

    seq_t term1 = 13 * n3;
    seq_t term2 = 3 * (3 * m + 1) * n2;
    seq_t term3 = (28 - 9 * m) * ns;
    seq_t term6 = 6 * (seq_t)r;

    seq_t numer = term1 + term2;
    numer -= term3;
    numer -= term6;
    return numer / 24;
}

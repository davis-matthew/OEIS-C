/*
 * A320577  Number of isosceles triangles whose vertices are the vertices of a
 *          regular n-gon.
 * https://oeis.org/A320577
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)/2 if n mod 6 = 1 or 5 a(n) = n*(n-2)/2 if n mod 6
 *            = 2 or 4 a(n) = n*(3n-7)/6 if n mod 6 = 3 a(n) = n*(3n-10)/6 if
 *            n mod 6 = 0
 * Range:     u8 n <= 24, u16 n <= 363, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                       /* offset starts at 3 */

    idx_t N = n;
    idx_t m = N % 6;
    idx_t res;

    switch (m) {
    case 1:
    case 5:
        res = N * (N - 1) / 2;
        break;
    case 2:
    case 4:
        res = N * (N - 2) / 2;
        break;
    case 3:
        res = N * (3 * N - 7) / 6;
        break;
    default: /* m == 0 */
        res = N * (3 * N - 10) / 6;
        break;
    }
    return (seq_t)res;
}

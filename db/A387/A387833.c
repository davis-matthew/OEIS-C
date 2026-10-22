/*
 * A387833  sqrt(a(n)) / 4 is the maximum area of any triangle with integer
 *          side lengths whose perimeter is n, or a(n) = -1 if there is no
 *          such triangle.
 * https://oeis.org/A387833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = if n % 3 == 0: 3 * (n/3)^4 if n % 3 == 1: 3*(n/3)^4 +
 *            4*(n/3)^3 - 2*(n/3)^2 - 4*(n/3) - 1 if n % 3 == 2: 3*(n/3)^4 +
 *            8*(n/3)^3 + 4*(n/3)^2 All arithmetic is performed in seq_t
 * Range:     i8 n <= 7, i16 n <= 30, i32 n <= 490, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m  = (seq_t)(n / 3);
    seq_t m2 = m * m;
    seq_t m3 = m2 * m;
    seq_t m4 = m3 * m;
    idx_t r  = n % 3;

    switch (r) {
    case 0:
        return 3 * m4;
    case 1:
        return 3 * m4 + 4 * m3 - 2 * m2 - 4 * m - 1;
    case 2:
        return 3 * m4 + 8 * m3 + 4 * m2;
    }
    return 0; /* unreachable */
}

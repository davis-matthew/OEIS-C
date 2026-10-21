/*
 * A283032  Number of inequivalent 5 X 5 matrices with entries in
 *          {1,2,3,...,n} up to vertical and horizontal reflections.
 * https://oeis.org/A283032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^13*(n^2 + 1)*(n^10 - n^8 + n^6 - n^4 + n^2 + 1)/4
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;        /* n^2 */
    seq_t n4 = n2 * n2;        /* n^4 */
    seq_t n8 = n4 * n4;        /* n^8 */
    seq_t n10 = n8 * n2;       /* n^10 */
    seq_t n6 = n4 * n2;        /* n^6 */
    seq_t n13 = 1;
    for (idx_t i = 0; i < 13; ++i) n13 *= sn; /* n^13 */

    seq_t p2 = n2 + 1;            /* n^2 + 1 */
    seq_t p3 = n10 - n8 + n6 - n4 + n2 + 1; /* n^10 - n^8 + n^6 - n^4 + n^2 + 1 */

    if ((sn & 1) == 0) {            /* even n: factor 4 in n^13 */
        seq_t part1 = n13 / 4;
        seq_t part2 = part1 * p2;
        return part2 * p3;
    } else {                         /* odd n: divide (n^2+1) and p3 by 2 each */
        seq_t part1 = n13;
        seq_t part2 = part1 * (p2 / 2);
        return part2 * (p3 / 2);
    }
}

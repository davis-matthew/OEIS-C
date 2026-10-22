/*
 * A389571  Number of rooted binary perfect phylogenies with sample size n and
 *          4 leaves.
 * https://oeis.org/A389571
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n-3)*(5*n-1)/48 for odd n; a(n) =
 *            n*(n-2)*(5*n-11)/48 + floor((n+4)/8) for even n
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 1423, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;
    seq_t nn = (seq_t)n;
    if (nn & 1) {                     /* odd n */
        seq_t part = ((nn - 1) * (nn - 3))
                     * (((seq_t)5 * nn) - 1);
        return part / 48;
    } else {                           /* even n */
        seq_t part = ((nn - 2) * nn)
                     * (((seq_t)5 * nn) - 11);
        part = part / 48;
        seq_t add = (nn + 4) / 8;
        return part + add;
    }
}

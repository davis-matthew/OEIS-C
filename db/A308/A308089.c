/*
 * A308089  Sum of the perimeters of all integer-sided triangles with
 *          perimeter n.
 * https://oeis.org/A308089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * A005044(n) where A005044(n) = round(n^2 / 48) if n is
 *            even, A005044(n) = round((n+3)^2 / 48) if n is odd The round of
 *            a/b for positive integers is (a + b/2) / b. For this sequence
 *            a(1)=0, a(2)=0
 * Range:     u8 n <= 12, u16 n <= 144, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* Use seq_t arithmetic from the beginning. */
    if ((n & 1) == 0) {            /* n is even */
        seq_t nn = (seq_t)n * (seq_t)n;          /* n^2 */
        seq_t triangles = (nn + 24) / 48;        /* round(n^2/48) */
        return (seq_t)n * triangles;
    } else {                       /* n is odd */
        seq_t np3 = (seq_t)n + 3;
        seq_t nn = np3 * np3;                      /* (n+3)^2 */
        seq_t triangles = (nn + 24) / 48;          /* round((n+3)^2/48) */
        return (seq_t)n * triangles;
    }
}

/*
 * A271998  Volatile sequence: a(n) = A018227(n)-6.
 * https://oeis.org/A271998
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 9 n^2 + 26 n - 24)/6 for even n a(n) = (n^3 + 9
 *            n^2 + 29 n - 15)/6 for odd n (offset: a(3) is the first term).
 *            This is equivalent to the recurrence a(n) =
 *            2·a(n‑1)+a(n‑2)-4·a(n‑3)+a(n‑4)+2·a(n‑5)-a(n‑6) for n>8
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                       /* values below the offset are ignored */

    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;                 /* n^2   */
    seq_t n3  = n2 * nn;                 /* n^3   */

    seq_t term;
    if ((nn & 1) == 0) {                 /* even n  */
        term = n3 + 9 * n2 + 26 * nn - 24;
    } else {                             /* odd n   */
        term = n3 + 9 * n2 + 29 * nn - 15;
    }
    return term / 6;
}

/*
 * A298032  Partial sums of A298031.
 * https://oeis.org/A298032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=15; for n>2 even a(n) = (9*n^2 - 6*n +
 *            2)/2; for n>2 odd a(n) = (9*n^2 - 6*n - 1)/2
 * Range:     u8 n <= 5, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return 1;
    if (n == 1u) return 5;
    if (n == 2u) return 15;

    seq_t nn = (seq_t)n;
    seq_t num = (seq_t)9 * nn * nn - (seq_t)6 * nn;
    if (n & 1u) {
        /* odd n */
        num -= 1;
    } else {
        /* even n */
        num += 2;
    }
    return num / 2;
}

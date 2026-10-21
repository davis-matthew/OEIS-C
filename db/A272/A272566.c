/*
 * A272566  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 501", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=9, a(2)=14, a(3)=58, a(4)=71; a(n) =
 *            (4*n^3+12*n^2-25*n+102)/6 if n>4 is even, a(n) =
 *            (4*n^3+24*n^2-25*n+123)/6 if n>4 is odd
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)58;
    if (n == 4) return (seq_t)71;

    seq_t nn = (seq_t)n;
    seq_t numer;
    if ((n & 1ULL) == 0) {                     /* even n */
        numer = 4*nn*nn*nn + 12*nn*nn - 25*nn + 102;
    } else {                                   /* odd n */
        numer = 4*nn*nn*nn + 24*nn*nn - 25*nn + 123;
    }
    return numer / 6;
}

/*
 * A265989  Number of n X 6 integer arrays with each element equal to the
 *          number of horizontal and antidiagonal neighbors not equal to
 *          itself.
 * https://oeis.org/A265989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=6, a(3)=9, a(4)=4 For n>4: if n is even: a(n) = 1 +
 *            2^(n-4) if n is odd : a(n) = 1 + 25 * 2^(n-5)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)4;

    if ((n & 1) == 0) {          /* even n */
        seq_t pow = 1;
        for (idx_t i = 0; i < n - 4; ++i) pow *= 2;
        return (seq_t)1 + pow;
    } else {                     /* odd n */
        seq_t pow = 1;
        for (idx_t i = 0; i < n - 5; ++i) pow *= 2;
        return (seq_t)1 + (seq_t)25 * pow;
    }
}

/*
 * A282033  An example of a collection of five sets (based on U.S. coinage)
 *          which is not an additive number system.
 * https://oeis.org/A282033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) for n>12. Initial terms: a(1)=1;
 *            a(2)=2; a(3)=3; a(4)=4; a(5)=5; a(6)=10; a(7)=20; a(8)=25;
 *            a(9)=50; a(10)=75; a(11)=100; a(12)=200. For n>=12 the closed
 *            form a(n)=100*n-1000 follows from the recurrence
 * Range:     u8 n <= 12, u16 n <= 665, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t small[12] = {
        (seq_t)1, 2, 3, 4, 5, 10, 20, 25, 50, 75, 100, 200
    };
    if (n <= 12) {
        return small[n - 1];
    }
    seq_t res = (seq_t)100 * (seq_t)n;
    res -= (seq_t)1000;
    return res;
}

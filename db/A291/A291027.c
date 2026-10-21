/*
 * A291027  p-INVERT of the positive integers, where p(S) = 1 - 5*S + S^2.
 * https://oeis.org/A291027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 17*a(n-2) + 9*a(n-3) - a(n-4) with initial
 *            values a(0)=5, a(1)=34, a(2)=226, a(3)=1501
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A291027.
 * seq_t and idx_t are defined by the build environment.
 * The implementation uses the linear recurrence of order 4
 * and does not cache values; recomputing up to n is O(n)
 * and fast for the required limits. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)34;
    if (n == 2) return (seq_t)226;
    if (n == 3) return (seq_t)1501;

    seq_t a0 = (seq_t)5;
    seq_t a1 = (seq_t)34;
    seq_t a2 = (seq_t)226;
    seq_t a3 = (seq_t)1501;
    seq_t a4;

    for (idx_t i = 4; i <= n; ++i) {
        a4 = 9 * a3;
        a4 -= 17 * a2;
        a4 += 9 * a1;
        a4 -= a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}

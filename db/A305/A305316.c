/*
 * A305316  a(n) = sqrt(5*b(n)^2 - 4) with b(n) = Fibonacci(6*n+5) =
 *          A134497(n).
 * https://oeis.org/A305316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - a(n-2) with a(0)=11, a(1)=199 (derived from
 *            a(n) = sqrt(5*F(6n+5)^2 - 4), but the linear recurrence allows
 *            an O(n) computation without Fibonacci or sqrt operations.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)11;
    if (n == 1) return (seq_t)199;

    /* Iterate recurrence */
    seq_t prevprev = (seq_t)11;    /* a(0) */
    seq_t prev = (seq_t)199;       /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t curr = prev * (seq_t)18 - prevprev;
        prevprev = prev;
        prev = curr;
    }
    return prev;
}

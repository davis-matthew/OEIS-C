/*
 * A279101  a(n) = Sum_{k=0..n} ceiling((1 + sqrt(2))^k).
 * https://oeis.org/A279101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..n} ceil((1+sqrt(2))^k) * Recurrence: a(n) =
 *            3*a(n-1) - 4*a(n-3) + a(n-4) + a(n-5) * a(0)=1, a(1)=4, a(2)=10,
 *            a(3)=25, a(4)=59
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for 0 ≤ n < 5 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)25;
    if (n == 4) return (seq_t)59;

    /* a1 = a(n-1), a2 = a(n-2), a3 = a(n-3),
       a4 = a(n-4), a5 = a(n-5) */
    seq_t a1 = (seq_t)59; /* a(4) */
    seq_t a2 = (seq_t)25; /* a(3) */
    seq_t a3 = (seq_t)10; /* a(2) */
    seq_t a4 = (seq_t)4;  /* a(1) */
    seq_t a5 = (seq_t)1;  /* a(0) */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 3*a(i-1) - 4*a(i-3) + a(i-4) + a(i-5) */
        next = 3 * a1;
        next -= 4 * a3;
        next += a4;
        next += a5;

        /* shift the window for the next iteration */
        a5 = a4;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = next;
    }
    return a1;
}

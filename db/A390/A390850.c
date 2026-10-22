/*
 * A390850  a(n) = Sum_{k=0..n} (-1)^k * binomial(n+k-1,n-k) * Fibonacci(k+1).
 * https://oeis.org/A390850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{k=0..n} (-1)^k * binomial(n+k-1, n-k) * Fibonacci(k+1)
 *            Recurrence: a(0)=1, a(1)=-1, a(2)=0, a(3)=2, a(4)=3 For n>4:
 *            a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - a(n-4)
 * Range:     i8 n <= 9, i16 n <= 16, i32 n <= 30, i64 n <= 59, i128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[5] = { 1, -1, 0, 2, 3 };

    if (n < 5) return init[n];

    seq_t a1 = init[1];   /* a(n-4) */
    seq_t a2 = init[2];   /* a(n-3) */
    seq_t a3 = init[3];   /* a(n-2) */
    seq_t a4 = init[4];   /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 3 * a4 - 3 * a3 + 3 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}

/*
 * A390824  a(n) = Sum_{k=0..n} binomial(n+k+2,n-k) * Fibonacci(k+1).
 * https://oeis.org/A390824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..n} binomial(n+k+2, n-k) * Fibonacci(k+1)
 *            Equivalent recurrence: a(0)=1, a(1)=4, a(2)=13, a(3)=42
 *            a(n)=5*a(n-1)-7*a(n-2)+5*a(n-3)-a(n-4) (n≥4)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)42;
 
    /* iterate using the 4‑term recurrence */
    seq_t a0 = (seq_t)1;     /* a(n-4) */
    seq_t a1 = (seq_t)4;     /* a(n-3) */
    seq_t a2 = (seq_t)13;    /* a(n-2) */
    seq_t a3 = (seq_t)42;    /* a(n-1) */
    seq_t ai;                /* a(n) */
 
    for (idx_t i = 4; i <= n; ++i) {
        ai = 5 * a3 - 7 * a2 + 5 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return ai;
}

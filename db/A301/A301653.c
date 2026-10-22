/*
 * A301653  Expansion of x*(1 + 2*x)/((1 - x)*(1 + x)*(1 - x - x^2)).
 * https://oeis.org/A301653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3) - a(n-4) with initial values
 *            a(0)=0, a(1)=1, a(2)=3, a(3)=5
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;

    /* Initialize the four previous terms */
    seq_t a0 = (seq_t)0; /* a(n-4) */
    seq_t a1 = (seq_t)1; /* a(n-3) */
    seq_t a2 = (seq_t)3; /* a(n-2) */
    seq_t a3 = (seq_t)5; /* a(n-1) */
    seq_t cur = a3;     /* placeholder for a(n) */

    /* Compute recurrence up to n */
    for (idx_t i = 4; i <= n; ++i) {
        cur = a3 + a2 * (seq_t)2 - a1 - a0;   /* a(i) = a(i-1)+2*a(i-2)-a(i-3)-a(i-4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }

    return cur;
}

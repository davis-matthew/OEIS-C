/*
 * A387847  a(n) = Sum_{k=0..n} binomial(3*n,5*k).
 * https://oeis.org/A387847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*n,5*k) Recurrence: a(n) =
 *            5*a(n-1) + 5*a(n-2) + 145*a(n-3) + 55*a(n-4) + 8*a(n-5) (n>4)
 *            Initial values: a(0)=1, a(1)=1, a(2)=7, a(3)=127, a(4)=859
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)7;
    if (n == 3u) return (seq_t)127;
    if (n == 4u) return (seq_t)859;

    /* sliding window of the last five terms */
    seq_t a0 = (seq_t)1;   /* a(n-5) */
    seq_t a1 = (seq_t)1;   /* a(n-4) */
    seq_t a2 = (seq_t)7;   /* a(n-3) */
    seq_t a3 = (seq_t)127; /* a(n-2) */
    seq_t a4 = (seq_t)859; /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t a5 = a4 * 5 + a3 * 5 + a2 * 145 + a1 * 55 + a0 * 8;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
    }
    return a4;
}

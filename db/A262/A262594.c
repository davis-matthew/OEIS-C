/*
 * A262594  Expansion of (1-2*x)^2/((1-x)^4*(1-4*x)).
 * https://oeis.org/A262594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1)-22*a(n-2)+28*a(n-3)-17*a(n-4)+4*a(n-5) * Initial
 *            values: a(0)=1, a(1)=4, a(2)=14, a(3)=52, a(4)=203
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)52;
    if (n == 4) return (seq_t)203;

    /* Iterate the linear recurrence for n >= 5 */
    seq_t a0 = (seq_t)1;   /* a(n-5) */
    seq_t a1 = (seq_t)4;   /* a(n-4) */
    seq_t a2 = (seq_t)14;  /* a(n-3) */
    seq_t a3 = (seq_t)52;  /* a(n-2) */
    seq_t a4 = (seq_t)203; /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t a5 = 8 * a4 - 22 * a3 + 28 * a2 - 17 * a1 + 4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
    }
    return a4;
}

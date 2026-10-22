/*
 * A386201  Lengths of the long leg in the unique primitive Pythagorean triple
 *          whose inradius is A000032(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A386201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386201
   Formula (linear recurrence): a(n) = 3*a(n-1) + a(n-2) - 5*a(n-3) - a(n-4) + a(n-5)
   Initial values (offset 0): a(0)=12, a(1)=4, a(2)=24, a(3)=40, a(4)=112
   This implements the recurrence directly and therefore works for any n ≥ 0
   without additional searches or tables. */

seq_t a(idx_t n)
{
    /* handle the first five terms */
    if (n == 0) return (seq_t)12;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)24;
    if (n == 3) return (seq_t)40;
    if (n == 4) return (seq_t)112;

    /* keep the last five values in a rolling window */
    seq_t a0 = 12;   /* a(n-5) */
    seq_t a1 = 4;    /* a(n-4) */
    seq_t a2 = 24;   /* a(n-3) */
    seq_t a3 = 40;   /* a(n-2) */
    seq_t a4 = 112;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t ai = 3 * a4 + a3 - 5 * a2 - a1 + a0;  /* a(i) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return a4; /* a(n) */
}

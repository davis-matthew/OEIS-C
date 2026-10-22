/*
 * A370846  a(n) = 5 + 4^n + 3*((-i*sqrt(7) - 3)^n + (i*sqrt(7) - 3)^n)/2^n.
 * https://oeis.org/A370846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) + 8*a(n-3) - 16*a(n-4), a(1) = 0,
 *            a(2) = 24, a(3) = 96, a(4) = 168
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)24;
    if (n == 3) return (seq_t)96;
    if (n == 4) return (seq_t)168;

    /* Use a recurrence with four previous values */
    seq_t a1 = (seq_t)0;       /* a(1) */
    seq_t a2 = (seq_t)24;      /* a(2) */
    seq_t a3 = (seq_t)96;      /* a(3) */
    seq_t a4 = (seq_t)168;     /* a(4) */
    seq_t curr;

    for (idx_t i = 5; i <= n; ++i)
    {
        curr = 2*a4 + 7*a3 + 8*a2 - 16*a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = curr;
    }

    return a4;
}

/*
 * A277667  Number of n-length words over a quaternary alphabet
 *          {a_1,a_2,...,a_4} avoiding consecutive letters a_i, a_{i+1}.
 * https://oeis.org/A277667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 3*a(n-2) + 2*a(n-3) - a(n-4),
   with a(0)=1, a(1)=4, a(2)=13, a(3)=42 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)42;

    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)4;   /* a(n-3) */
    seq_t a2 = (seq_t)13;  /* a(n-2) */
    seq_t a3 = (seq_t)42;  /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* next = 4*a3 + 2*a1 - (3*a2 + a0) */
        seq_t t1 = a3; t1 *= (seq_t)4;            /* 4*a3 */
        seq_t t2 = a1; t2 *= (seq_t)2;            /* 2*a1 */
        seq_t next = t1 + t2;

        seq_t s1 = a2; s1 *= (seq_t)3;            /* 3*a2 */
        seq_t s2 = a0;                            /* a0 */
        seq_t sub = s1 + s2;                      /* 3*a2 + a0 */
        next -= sub;

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}

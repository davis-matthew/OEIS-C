/*
 * A270863  Self-composition of the Fibonacci sequence.
 * https://oeis.org/A270863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=2, a(3)=6,
 *            a(n)=3*a(n-1)+a(n-2)-3*a(n-3)-a(n-4) for n>3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 6;

    /* Iteratively compute using the recurrence */
    seq_t a0 = 0, a1 = 1, a2 = 2, a3 = 6;
    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = 3 * a3 + a2;
        next -= 3 * a1 + a0; /* safe subtraction because the sequence is increasing */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}

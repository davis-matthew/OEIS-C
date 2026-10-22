/*
 * A334771  a(n) = smallest m that generates a rotationally symmetrical
 *          XOR-triangle T(m) with a central triangle of zeros with side
 *          length n.
 * https://oeis.org/A334771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-4) - 16*a(n-8), with initial values a(1)=543,
 *            a(2)=151, a(3)=2359, a(4)=599, a(5)=8607, a(6)=2391, a(7)=37687,
 *            a(8)=9559
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 1; terms for n < 1 are undefined – return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Array holding the last eight terms; indices 0..7 correspond to
       a(k), a(k+1), …, a(k+7). */
    seq_t buf[8] = {543, 151, 2359, 599, 8607, 2391, 37687, 9559};

    /* For n ≤ 8 the value is known directly. */
    if (n <= 8)
        return buf[n - 1];

    /* Compute successive terms using the recurrence.  We keep a
       circular buffer in 'buf' – at step i (≥ 9) buf[(i-1)%8] holds
       a(i-1).  The indices for the recurrence are computed modulo 8. */
    for (idx_t i = 9; i <= n; ++i)
    {
        /* (i-5)%8 = index of a(i-4), (i-9)%8 = index of a(i-8). */
        seq_t next = (seq_t)17 * buf[(i - 5) % 8] - (seq_t)16 * buf[(i - 9) % 8];
        buf[(i - 1) % 8] = next;
    }

    return buf[(n - 1) % 8];
}

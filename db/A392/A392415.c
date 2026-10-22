/*
 * A392415  Number of partitions of the vertices of the n-ladder graph into
 *          total dominating sets.
 * https://oeis.org/A392415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-3) + a(n-4) Initial values: a(1)=1,
 *            a(2)=3, a(3)=3, a(4)=9 The computation uses only integer
 *            arithmetic on seq_t
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 1) return 1;
    if (n == 2 || n == 3) return 3;
    if (n == 4) return 9;

    /* a0  = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1)   */
    seq_t a0 = 1;   /* a(1)   */
    seq_t a1 = 3;   /* a(2)   */
    seq_t a2 = 3;   /* a(3)   */
    seq_t a3 = 9;   /* a(4)   */

    /* Iterate from 5 up to n */
    for (idx_t i = 5; i <= n; ++i) {
        /* Compute 3*a3 and 3*a1 using only addition */
        seq_t three_a3 = a3; three_a3 += a3; three_a3 += a3;  /* 3*a3   */
        seq_t three_a1 = a1; three_a1 += a1; three_a1 += a1;  /* 3*a1   */

        /* next = 3*a3 - 3*a1 + a0 */
        seq_t next = three_a3 - three_a1;
        next += a0;

        /* Shift values for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;  /* a(n) */
}

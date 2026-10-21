/*
 * A250754  Number of (n+1) X (7+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (468*3^n - 224*2^n + 98) / 2 Recurrence: a(n) = 6*a(n-1)
 *            - 11*a(n-2) + 6*a(n-3) Base values: a(1) = 527, a(2) = 1707,
 *            a(3) = 5471
 * Range:     u8 none, u16 n <= 5, u32 n <= 15, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. */
    if (n < 1) {
        /* Undefined for n < 1; return 0 as a safe default. */
        return 0;
    }

    /* Return known base values directly. */
    if (n == 1) return 527;
    if (n == 2) return 1707;
    if (n == 3) return 5471;

    /* Use a recurrence that depends only on the last three values. */
    seq_t a1 = 527;   /* a(n-3) */
    seq_t a2 = 1707;  /* a(n-2) */
    seq_t a3 = 5471;  /* a(n-1) */
    seq_t next;

    /* Compute through to the requested index. */
    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 6*a(i-1) - 11*a(i-2) + 6*a(i-3) */
        next = 6 * a3 - 11 * a2 + 6 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}

/*
 * A267530  Number of OFF (white) cells in the n-th iteration of the "Rule
 *          141" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) with a(0)=0, a(1)=2, a(2)=3,
 *            a(3)=4, a(4)=4
 * Range:     u8 n <= 506, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    const seq_t init[5] = {0, 2, 3, 4, 4};
    if (n < 5)
        return init[n];

    /* a_{n-3}, a_{n-2}, a_{n-1} for the current n */
    seq_t a_nm3 = init[2];   /* a(2) */
    seq_t a_nm2 = init[3];   /* a(3) */
    seq_t a_nm1 = init[4];   /* a(4) */

    /* Iterate from n = 5 up to the desired index */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t a_n = a_nm1 + a_nm2 - a_nm3; /* recurrence */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}

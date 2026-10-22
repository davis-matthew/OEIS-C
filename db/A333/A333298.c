/*
 * A333298  Number of canonical sequences of moves of length n for the Rubik
 *          cube puzzle using the half-turn metric.
 * https://oeis.org/A333298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) + 18*a(n-2) for n>2 * Initial values: a(0)=1,
 *            a(1)=18, a(2)=243
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 0) return 1;
    if (n == 1) return 18;
    if (n == 2) return 243;

    /* Iterate the linear recurrence.
       a_nm2 holds a(n-2), a_nm1 holds a(n-1). */
    seq_t a_nm2 = 18;   /* a(1) */
    seq_t a_nm1 = 243;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t a_n = 12 * a_nm1 + 18 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;  /* a(n) */
}

/*
 * A322054  Number of decimal strings of length n that do not contain a
 *          specific string xx (where x is a single digit).
 * https://oeis.org/A322054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*a(n-1) + 9*a(n-2) Initial values: a(1) = 10, a(2) = 99
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases explicitly */
    if (n == 1)
        return (seq_t)10;
    if (n == 2)
        return (seq_t)99;

    /* Compute iteratively using the recurrence */
    seq_t a_nm2 = (seq_t)10;  /* a(1) */
    seq_t a_nm1 = (seq_t)99;  /* a(2) */
    seq_t a_n   = a_nm1;      /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        a_n  = a_nm1 * (seq_t)9 + a_nm2 * (seq_t)9;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}

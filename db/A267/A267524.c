/*
 * A267524  Binary representation of the middle column of the "Rule 139"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 10*a(n-2) for n > 3 Base terms: a(0) = 1,
 *            a(1) = 10, a(2) = 100, a(3) = 1001 The recurrence generates the
 *            binary representation of the middle column of Rule 139 starting
 *            with a single ON cell
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four indices explicitly. */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)10;
    if (n == (idx_t)2) return (seq_t)100;
    if (n == (idx_t)3) return (seq_t)1001;

    /* Compute the sequence iteratively for n >= 4. */
    seq_t a_nm2 = (seq_t)100;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)1001;  /* a(n-1) */
    seq_t a_n   = a_nm1;        /* placeholder */

    for (idx_t i = 4; i <= n; ++i) {
        a_n = a_nm1 * (seq_t)11 - a_nm2 * (seq_t)10;
        a_nm2 = a_nm1;   /* shift indices for next step */
        a_nm1 = a_n;
    }

    return a_n;
}

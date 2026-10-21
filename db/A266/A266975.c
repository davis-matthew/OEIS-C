/*
 * A266975  Binary representation of the n-th iteration of the "Rule 78"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + 10000*a(n-2) - 100000*a(n-3) for n > 3
 *            Initial values: a(0) = 1 a(1) = 110 a(2) = 11100 a(3) = 1101000
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)110;
    if (n == 2) return (seq_t)11100;
    if (n == 3) return (seq_t)1101000;

    /* a_nm3 = a(n-3), a_nm2 = a(n-2), a_nm1 = a(n-1) for the current loop
       initialise them with the values needed for i = 4 */
    seq_t a_nm3 = (seq_t)110;         /* a(1) */
    seq_t a_nm2 = (seq_t)11100;       /* a(2) */
    seq_t a_nm1 = (seq_t)1101000;     /* a(3) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = a_nm1 * (seq_t)10 + a_nm2 * (seq_t)10000 - a_nm3 * (seq_t)100000;
        /* shift the window for the next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}

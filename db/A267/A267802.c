/*
 * A267802  Decimal representation of the n-th iteration of the "Rule 213"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2), a(0)=1, a(1)=3, a(2)=19, a(3)=115
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)19;
    if (n == 3) return (seq_t)115;

    /* For n > 3 use the linear recurrence.  All arithmetic is
       performed modulo the capacity of seq_t. */
    seq_t a_nm2 = (seq_t)19;   /* a(2) */
    seq_t a_nm1 = (seq_t)115;  /* a(3) */
    seq_t a_n;

    for (idx_t i = 4; i <= n; ++i) {
        a_n = (seq_t)(5 * a_nm1) - (seq_t)(4 * a_nm2);
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}

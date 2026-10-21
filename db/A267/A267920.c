/*
 * A267920  Binary representation of the n-th iteration of the "Rule 243"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 101*a(n-1) - 100*a(n-2) a(0) = 1, a(1) = 101, a(2) =
 *            11101
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)101;
    if (n == 2) return (seq_t)11101;

    seq_t a_nm2 = (seq_t)101;   /* a(1) */
    seq_t a_nm1 = (seq_t)11101; /* a(2) */
    seq_t a_n = 0;

    for (idx_t i = 3; i <= n; ++i) {
        a_n = (seq_t)101 * a_nm1 - (seq_t)100 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}

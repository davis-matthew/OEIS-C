/*
 * A266247  Binary representation of the middle column of the "Rule 9"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-2) - 10*a(n-3) for n > 6 (with a(0)=1,
 *            a(1)=10, a(2)=101, a(3)=1010, a(4)=10101, a(5)=101011,
 *            a(6)=1010110)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for n = 0 … 6 */
    static const seq_t init[7] = {1, 10, 101, 1010, 10101, 101011, 1010110};

    if (n <= 6) {
        return init[n];
    }

    /* start recurrence from n = 7, with a(4), a(5), a(6) as the last three terms */
    seq_t a_nm3 = init[4];  /* a(n-3) when n = 7   */
    seq_t a_nm2 = init[5];  /* a(n-2)              */
    seq_t a_nm1 = init[6];  /* a(n-1)              */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t a_n = 10 * a_nm1 + a_nm2 - 10 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}

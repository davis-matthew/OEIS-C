/*
 * A275267  Number of 3 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,1) or (-1,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 2*a(n-2) - 6*a(n-3) for n>4; initial values:
 *            a(1)=5, a(2)=24, a(3)=128, a(4)=688
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases for n = 1..4 */
    if (n <= 4) {
        static const seq_t base[4] = {5, 24, 128, 688};
        return base[n - 1];
    }

    /* Compute iteratively using the recurrence relation.  */
    seq_t a_nm1 = 688;   /* a(4)  */
    seq_t a_nm2 = 128;   /* a(3)  */
    seq_t a_nm3 = 24;    /* a(2)  */
    seq_t current = 0;   /* placeholder for a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        current = 6 * a_nm1 - 2 * a_nm2 - 6 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = current;
    }

    return current;
}

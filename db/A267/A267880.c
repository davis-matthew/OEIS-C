/*
 * A267880  Decimal representation of the middle column of the "Rule 233"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) for n > 4 with initial values a(0)=1,
 *            a(1)=2, a(2)=5, a(3)=10, a(4)=21
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small fixed initial values */
    static const seq_t init[5] = { 1, 2, 5, 10, 21 };

    if (n < 5)
        return init[n];

    /* a(3) and a(4) are the starting values for the recurrence */
    seq_t a_nm2 = init[3];    /* a(n-2)  for n = 5  */
    seq_t a_nm1 = init[4];    /* a(n-1)  for n = 5  */
    seq_t a_n   = 0;          /* a(n)    */

    for (idx_t i = 5; i <= n; ++i)
    {
        a_n = ((seq_t)3 * a_nm1) - ((seq_t)2 * a_nm2);
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}

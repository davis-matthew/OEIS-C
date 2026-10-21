/*
 * A284483  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 961", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A284483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=111, a(3)=1101, a(4)=11110, a(5)=111101;
 *            For n>5: a(n) = 10*a(n-1) + a(n-2) - 10*a(n-3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n)
    {
        case 0: return (seq_t)1;
        case 1: return (seq_t)0;
        case 2: return (seq_t)111;
        case 3: return (seq_t)1101;
        case 4: return (seq_t)11110;
        case 5: return (seq_t)111101;
        default:
        {
            seq_t a_nm3 = (seq_t)1101;   /* a(3) */
            seq_t a_nm2 = (seq_t)11110;  /* a(4) */
            seq_t a_nm1 = (seq_t)111101; /* a(5) */
            seq_t a_n;

            for (idx_t i = 6; i <= n; ++i)
            {
                a_n = ((seq_t)10 * a_nm1) + a_nm2 - ((seq_t)10 * a_nm3);
                a_nm3 = a_nm2;
                a_nm2 = a_nm1;
                a_nm1 = a_n;
            }
            return a_nm1;
        }
    }
}

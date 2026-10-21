/*
 * A267588  Binary representation of the middle column of the "Rule 169"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 10*a(n-2) for n > 5, with initial values
 *            a(0)=1, a(1)=10, a(2)=101, a(3)=1010, a(4)=10100, a(5)=101001
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
    case 0: return (seq_t)1;
    case 1: return (seq_t)10;
    case 2: return (seq_t)101;
    case 3: return (seq_t)1010;
    case 4: return (seq_t)10100;
    case 5: return (seq_t)101001;
    default:
        /* iterate from a(4) and a(5) using the linear recurrence */
        seq_t a_nm2 = (seq_t)10100;   /* a(n-2), starts at a(4)   */
        seq_t a_nm1 = (seq_t)101001;  /* a(n-1), starts at a(5)   */
        seq_t a_n;

        for (idx_t i = 6; i <= n; ++i) {
            a_n = (seq_t)11 * a_nm1 - (seq_t)10 * a_nm2;
            a_nm2 = a_nm1;
            a_nm1 = a_n;
        }
        return a_nm1;
    }
}

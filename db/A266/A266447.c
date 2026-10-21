/*
 * A266447  Number of ON (black) cells in the n-th iteration of the "Rule 25"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   • a(0)=1, a(1)=1, a(2)=2, a(3)=3 (initial values) • for n>=4: if
 *            n is even: a(n)=4 if n is odd : a(n)=2*n-4 This follows from
 *            a(n)=n-(-1)^n*(n-4) for n>3
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values for n = 0,1,2,3 */
    static const seq_t init[4] = {(seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3};

    if (n < 4)
        return init[n];

    /* For n >= 4:
       even n -> 4
       odd  n -> 2*n-4                                    */
    if ((n & 1) == 0)
        return (seq_t)4;
    else
        return (seq_t)((seq_t)n * ((seq_t)2) - (seq_t)4);
}

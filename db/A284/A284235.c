/*
 * A284235  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 833", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A284235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-2) - 10*a(n-3) for n > 5; Initial terms:
 *            a(0) = 1 a(1) = 0 a(2) = 11 a(3) = 101 a(4) = 1010 a(5) = 10101
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)0;
        case 2: return (seq_t)11;
        case 3: return (seq_t)101;
        case 4: return (seq_t)1010;
        case 5: return (seq_t)10101;
    }

    seq_t a_nm3 = (seq_t)101;   /* a(3) */
    seq_t a_nm2 = (seq_t)1010;  /* a(4) */
    seq_t a_nm1 = (seq_t)10101; /* a(5) */
    seq_t a_i = 0;

    for (idx_t i = 6; i <= n; ++i) {
        a_i = a_nm1 * (seq_t)10 + a_nm2 - a_nm3 * (seq_t)10;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }

    return a_i;
}

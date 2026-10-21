/*
 * A289099  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 545", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A289099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3) for n > 5 Initial values:
 *            a(0)=1, a(1)=1, a(2)=7, a(3)=7, a(4)=23, a(5)=31
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first six terms explicitly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)7;
    if (n == 3u) return (seq_t)7;
    if (n == 4u) return (seq_t)23;
    if (n == 5u) return (seq_t)31;

    /* Iterate using the recurrence */
    seq_t a_n_3 = (seq_t)7;  /* a(3) */
    seq_t a_n_2 = (seq_t)23; /* a(4) */
    seq_t a_n_1 = (seq_t)31; /* a(5) */
    seq_t a_cur  = 0;        /* will hold a(n) */

    for (idx_t i = 6; i <= n; ++i) {
        a_cur = a_n_1 + ((seq_t)4 * a_n_2) - ((seq_t)4 * a_n_3);
        a_n_3 = a_n_2;
        a_n_2 = a_n_1;
        a_n_1 = a_cur;
    }
    return a_cur;
}

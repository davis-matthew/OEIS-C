/*
 * A284485  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 961", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A284485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(1) = 0; a(2) = 7; * a(n) = a(n-1) + 4*a(n-2) -
 *            4*a(n-3) for n > 5 * (This recurrence yields the closed‑form
 *            a(n) = (-4-(-2)^n+5*2^n)/4 for n>2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the small predefined terms. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)11;
    if (n == 4) return (seq_t)15;
    if (n == 5) return (seq_t)47;

    /* Use the 3‑term recurrence for n ≥ 6. */
    seq_t a_nm3 = (seq_t)11;   /* a(3) */
    seq_t a_nm2 = (seq_t)15;   /* a(4) */
    seq_t a_nm1 = (seq_t)47;   /* a(5) */
    seq_t cur;

    for (idx_t i = 6; i <= n; ++i) {
        cur = a_nm1 + 4 * a_nm2 - 4 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}

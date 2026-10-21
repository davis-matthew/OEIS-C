/*
 * A267604  Decimal representation of the middle column of the "Rule 175"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) for n > 3 with a(0) = 1, a(1) = 3,
 *            a(2) = 6, a(3) = 13 (This is A267604: Decimal representation of
 *            the middle column of Rule 175 elementary cellular automaton.)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* pre‑initialized first four terms */
    static const seq_t init[4] = { (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)13 };

    if (n < 4)
        return init[n];

    seq_t a_nm2 = init[2];   /* a(n-2) */
    seq_t a_nm1 = init[3];   /* a(n-1) */
    seq_t a_n;               /* a(n) */

    /* iterate upwards from 4 up to n */
    for (idx_t i = 4; i <= n; ++i) {
        a_n = 3 * a_nm1 - 2 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1; /* this holds a(n) */
}

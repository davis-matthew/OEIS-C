/*
 * A300438  Expansion of (1+x)^3/(1-x-2*x^2-x^3+x^4).
 * https://oeis.org/A300438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 2*a(n-2) + a(n-3) - a(n-4)  for n ≥ 4
   Prefix values: a(0)=1, a(1)=4, a(2)=9, a(3)=19   */
seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 9;
    if (n == 3) return 19;

    /* Variables to hold the last four values:
       a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1)   */
    seq_t a0 = 1, a1 = 4, a2 = 9, a3 = 19, a_next;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = a3 + 2*a2 + a1 - a0  */
        a_next = a3 + ((seq_t)2) * a2 + a1 - a0;

        /* Shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a_next;
    }
    return a3;
}

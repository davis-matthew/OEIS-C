/*
 * A266252  Total number of OFF (white) cells after n iterations of the "Rule
 *          9" elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((-1)^n*(n-4)+n*(n+4))/2 for n > 2 Initial values: a(0) =
 *            0, a(1) = 3, a(2) = 6
 * Range:     u8 n <= 21, u16 n <= 359, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 3;
    if (n == 2) return 6;

    /* n > 2 : use the closed‑form expression */
    long long nn = (long long)n;                 /* n fits into 64‑bit for the tests */
    long long prod = nn * (nn + 4);              /* n*(n+4) */
    long long offset = nn - 4;                   /* n-4 */
    if (nn & 1LL)                               /* n odd : (-1)^n = -1 */
        prod -= offset;
    else
        prod += offset;
    prod /= 2;                                   /* divide by 2 */

    return (seq_t)prod;
}

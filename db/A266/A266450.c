/*
 * A266450  Total number of OFF (white) cells after n iterations of the "Rule
 *          25" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 + 2*((-1)^n+3)*n - 7*(-1)^n - 1)/4 for n > 2 a(0)
 *            = 0, a(1) = 2, a(2) = 5
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the special cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;

    /* for n >= 3 use the closed‑form formula */
    long long nn   = (long long)n;           /* safe: n <= 10000 in tests */
    int      sign  = (n & 1) ? -1 : 1;       /* (-1)^n */

    long long num =
        2LL * nn * nn                     /* 2*n^2 */
      + 2LL * (sign + 3) * nn             /* 2*((-1)^n+3)*n */
      - 7LL * sign                         /* -7*(-1)^n */
      - 1;                                 /* -1 */

    /* division by 4 yields an integer */
    return (seq_t)(num / 4);
}

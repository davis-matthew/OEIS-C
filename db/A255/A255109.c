/*
 * A255109  Number of length n+2 0..2 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical polynomial:
 * a(n) = (n^5 + 25*n^4 + 185*n^3 + 875*n^2 + 1434*n + 720) / 120
 * (valid for all n >= 1)
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for offset 1, but safe fallback */

    __int128 x   = (__int128) n;      /* use 128‑bit intermediate to avoid overflow */
    __int128 num = 1;                 /* coefficient of n^5 */

    num = num * x + 25;               /* + 25 n^4   */
    num = num * x + 185;              /* + 185 n^3  */
    num = num * x + 875;              /* + 875 n^2  */
    num = num * x + 1434;             /* + 1434 n   */
    num = num * x + 720;              /* + 720      */

    seq_t result = (seq_t)(num / 120);/* integer division yields exact value */

    return result;
}

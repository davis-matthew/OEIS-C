/*
 * A255177  Second differences of seventh powers (A001015).
 * https://oeis.org/A255177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0, else 14*n*(3*n^4 + 5*n^2 + 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 39, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the closed‑form formula for OEIS A255177.  */
seq_t a(idx_t n)
{
    if (n == 0)           /* a(0) = 1 */
        return 1;

    /* Use a wider type for intermediate arithmetic to avoid overflow. */
    unsigned __int128 nt = n;
    unsigned __int128 n2 = nt * nt;        /* n^2   */
    unsigned __int128 n4 = n2 * n2;        /* n^4   */
    unsigned __int128 part = 3 * n4 + 5 * n2 + 1;
    unsigned __int128 res  = 14 * nt * part;

    return (seq_t)res;
}

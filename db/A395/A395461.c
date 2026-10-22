/*
 * A395461  Expansion of e.g.f. x^3*(1 + x + x^2/2 + x^3/6)*exp(x).
 * https://oeis.org/A395461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)^2*(n^2 - 7*n + 18)/6
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* Use 128‑bit arithmetic to avoid intermediate overflow. */
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 term = N * (N - 1) * (N - 2) * (N - 2)
                           * (N * N - 7 * N + 18);
    term /= 6;

    return (seq_t)term;
}

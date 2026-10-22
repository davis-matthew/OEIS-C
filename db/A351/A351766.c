/*
 * A351766  a(n) = Sum_{j=1..n} Sum_{i=1..j} (i*j)^4.
 * https://oeis.org/A351766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(2*n+1)*(2*n+3)*(9*n^5 + 25*n^4 - 5*n^3 -
 *            25*n^2 + 21*n - 5)/1800
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Special‑case the zero offset. */
    if (n == 0)
        return 0;

    /* Compute all terms using a 128‑bit unsigned intermediate. */
    unsigned __int128 N   = n;
    unsigned __int128 n2  = N * N;           /* n^2 */
    unsigned __int128 n3  = n2 * N;          /* n^3 */
    unsigned __int128 n4  = n3 * N;          /* n^4 */
    unsigned __int128 n5  = n4 * N;          /* n^5 */

    /* Polynomial part: 9*n^5 + 25*n^4 - 5*n^3 - 25*n^2 + 21*n - 5 */
    unsigned __int128 poly = 9 * n5 + 25 * n4 - 5 * n3 - 25 * n2 + 21 * N - 5;

    /* Product of the five combinatorial factors. */
    unsigned __int128 num = N * (N + 1) * (N + 2);
    num *= (2 * N + 1) * (2 * N + 3);
    num *= poly;      /* multiply with the polynomial */

    /* Divide by 1800 to obtain the exact integer result. */
    unsigned __int128 res = num / 1800u;

    return (seq_t)res;
}

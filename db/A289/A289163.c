/*
 * A289163  Number of 5-cycles in the n X n black bishop graph.
 * https://oeis.org/A289163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-165 - 282*n + 972*n^2 - 730*n^3 + 280*n^4 - 68*n^5 +
 *            8*n^6 * - 5*(-1)^n*(-33 - 26*n + 96*n^2 - 46*n^3 + 6*n^4)) / 240
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 72, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases: first 12 terms */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)0;
    if (n == 4) return (seq_t)10;
    if (n == 5) return (seq_t)84;
    if (n == 6) return (seq_t)322;
    if (n == 7) return (seq_t)1172;
    if (n == 8) return (seq_t)2780;
    if (n == 9) return (seq_t)7016;
    if (n ==10) return (seq_t)13532;
    if (n ==11) return (seq_t)27720;
    if (n ==12) return (seq_t)47318;

    /* Use 128‑bit arithmetic for safe intermediate values (works for GCC/Clang). */
    signed __int128 NN = (signed __int128)n;          /* cast index to signed 128 */
    signed __int128 p1 = 8;           /* Horner for 8*n^6 - 68*n^5 + 280*n^4 - 730*n^3 + 972*n^2 - 282*n - 165 */
    p1 = p1 * NN + (-68);
    p1 = p1 * NN + 280;
    p1 = p1 * NN + (-730);
    p1 = p1 * NN + 972;
    p1 = p1 * NN + (-282);
    p1 = p1 * NN + (-165);

    signed __int128 p2 = 6;           /* Horner for 6*n^4 - 46*n^3 + 96*n^2 - 26*n - 33 */
    p2 = p2 * NN + (-46);
    p2 = p2 * NN + 96;
    p2 = p2 * NN + (-26);
    p2 = p2 * NN + (-33);

    /* (-1)^n */
    int s = (n & 1) ? -1 : 1;

    signed __int128 total = p1 - ( (__int128)5 * s * p2 );
    total = total / 240;   /* exact division, result fits in seq_t for all n where it is representable */

    return (seq_t)total;
}

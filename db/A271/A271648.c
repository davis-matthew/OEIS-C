/*
 * A271648  Number of permutations in S_{2*n+3} containing the pattern
 *          2143...(2n)(2n-1).
 * https://oeis.org/A271648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 6, a(1) = 119; For n ≥ 2, a(n) = ((32*n^6 + 80*n^4 +
 *            16*n^3 + 38*n^2 + 59*n)/3) + 32*n^5 + 13
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;
    if (n == 1) return (seq_t)119;

    /* Use 128‑bit arithmetic for the intermediate calculations. */
    unsigned __int128 N  = (unsigned __int128)(unsigned long long)n;
    unsigned __int128 n2 = N * N;          /* n² */
    unsigned __int128 n3 = n2 * N;         /* n³ */
    unsigned __int128 n4 = n2 * n2;        /* n⁴ */
    unsigned __int128 n5 = n4 * N;         /* n⁵ */
    unsigned __int128 n6 = n3 * n3;        /* n⁶ */

    unsigned __int128 part = 32 * n6 + 80 * n4 + 16 * n3 + 38 * n2 + 59 * N;
    unsigned __int128 res  = part / 3 + 32 * n5 + 13;

    return (seq_t)res;
}

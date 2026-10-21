/*
 * A296998  Number of ways to place 4 points on an n X n point grid so that no
 *          point is equally distant from two other points on the same row or
 *          the same column.
 * https://oeis.org/A296998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 - 6*n^6 - 12*n^5 + 35*n^4 + 56*n^3 - 150*n^2)/24 +
 *            b(n), where b(n) is periodic with modulus 12: - b(n) = 0 for n ≡
 *            0 (mod 12) - b(n) = -n^3/2 + 11*n/3 for n ≡ 1,5,7,11 (mod 12) -
 *            b(n) = 8*n/3 for n ≡ 2,10 (mod 12) - b(n) = -n^3/2 + 3*n for n ≡
 *            3,9 (mod 12) - b(n) = 2*n/3 for n ≡ 4,8...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 23, u64 n <= 256, u128 n <= 256,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow; the result is finally
       converted back to seq_t.                                          */
    __int128 N  = (__int128)n;
    __int128 N2 = N * N;
    __int128 N3 = N2 * N;
    __int128 N4 = N3 * N;
    __int128 N5 = N4 * N;
    __int128 N6 = N5 * N;
    __int128 N8 = N4 * N4;          /* n^8 = (n^4)^2 */

    __int128 num = N8
                 - (__int128)6 *  N6
                 - (__int128)12 * N5
                 + (__int128)35 * N4
                 + (__int128)56 * N3
                 - (__int128)150 * N2;

    uint64_t mod = n % 12;
    __int128 B = 0;
    switch (mod) {
        case 0:      B = 0;                     break;
        case 1: case 5: case 7: case 11:
            B = -(__int128)12 * N3 + (__int128)88 * N; break;
        case 2: case 10:
            B = (__int128)64 * N;                break;
        case 3: case 9:
            B = -(__int128)12 * N3 + (__int128)72 * N; break;
        case 4: case 8:
            B = (__int128)16 * N;                break;
        case 6:
            B = (__int128)48 * N;                break;
    }

    num += B;                       /* total numerator (divisible by 24) */
    return (seq_t)(num / 24);       /* integer division yields a(n)     */
}

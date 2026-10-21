/*
 * A286446  Number of non-equivalent ways to tile an n X n X n triangular area
 *          with four 2 X 2 X 2 triangular tiles and an appropriate number (=
 *          n^2-16) of 1 X 1 X 1 tiles.
 * https://oeis.org/A286446
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n^8 -12*n^7 +6*n^6 +432*n^5 -1249*n^4 -5028*n^3
 *            +21820*n^2 +12384*n -94000)/144) * + IF(n mod 2 = 1, (-8*n^3
 *            +72*n^2 -208*n +189)/24,0) * + IF(n mod 3 = 0, (-n^2 +3*n
 *            +7)/9,0) * for n ≥ 5; a(3)=0, a(4)=1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 31, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset explicitly */
    if (n == 3) return 0;
    if (n == 4) return 1;
    if (n < 3) return 0;          /* unused, but keeps function defined for all n */

    /* use signed 128‑bit arithmetic when available for intermediate calculations */
#if defined(__SIZEOF_INT128__)
    __int128 N = (__int128)n;

    __int128 n2 = N * N;
    __int128 n3 = n2 * N;
    __int128 n4 = n2 * n2;
    __int128 n5 = n4 * N;
    __int128 n6 = n5 * N;
    __int128 n7 = n6 * N;
    __int128 n8 = n7 * N;

    __int128 num1 =  n8
                   - (__int128)12 * n7
                   + (__int128)6  * n6
                   + (__int128)432 * n5
                   - (__int128)1249 * n4
                   - (__int128)5028 * n3
                   + (__int128)21820 * n2
                   + (__int128)12384 * N
                   - (__int128)94000;

    __int128 num2 = 0;
    if ((n & 1ULL) != 0) {
        num2 = -(__int128)8   * n3
               + (__int128)72  * n2
               - (__int128)208 * N
               + (__int128)189;
    }

    __int128 num3 = 0;
    if ((n % 3) == 0) {
        num3 = -(__int128)n2
               + (__int128)3 * N
               + (__int128)7;
    }

    /* common denominator 144: 144/24 = 6, 144/9 = 16 */
    __int128 total = num1 + (num2 * 6) + (num3 * 16);
    __int128 result = total / 144;

    return (seq_t)result;
#else
    /* Fallback using seq_t arithmetic – works for arbitrary‑precision seq_t */
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n2 * n2;
    seq_t n5 = n4 * N;
    seq_t n6 = n5 * N;
    seq_t n7 = n6 * N;
    seq_t n8 = n7 * N;

    seq_t num1 =  n8
                - (seq_t)12 * n7
                + (seq_t)6  * n6
                + (seq_t)432 * n5
                - (seq_t)1249 * n4
                - (seq_t)5028 * n3
                + (seq_t)21820 * n2
                + (seq_t)12384 * N
                - (seq_t)94000;

    seq_t num2 = 0;
    if ((n & 1ULL) != 0) {
        num2 = - (seq_t)8   * n3
               + (seq_t)72  * n2
               - (seq_t)208 * N
               + (seq_t)189;
    }

    seq_t num3 = 0;
    if ((n % 3) == 0) {
        num3 = - (seq_t)n2
               + (seq_t)3 * N
               + (seq_t)7;
    }

    /* common denominator 144: 144/24 = 6, 144/9 = 16 */
    seq_t total = num1 + (num2 * 6) + (num3 * 16);
    return total / 144;
#endif
}

/*
 * A255664  Number of length n+4 0..3 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/39916800)*(163 + Σ_{e=1}^{11} c_e * n^e) (n>2) where
 *            c_1 = 13230488160, c_2 = 5635820520, c_3 = 4673177916, c_4 =
 *            1984516270, c_5 = 772719695, c_6 = 83453370, c_7 = 4671183, c_8
 *            = 152130, c_9 = 5445, c_10 = 110, c_11 = 1. Initial values: a(1)
 *            = 1024, a(2) = 4096. The polynomial holds fo...
 * Range:     u8 none, u16 n <= 4, u32 n <= 26, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1024;
    if (n == 2) return (seq_t)4096;

    static const unsigned long long coef[12] = {
        0ULL,                     /* placeholder for e = 0 */
        13230488160ULL,           /* e = 1 */
        5635820520ULL,            /* e = 2 */
        4673177916ULL,            /* e = 3 */
        1984516270ULL,            /* e = 4 */
        772719695ULL,             /* e = 5 */
        83453370ULL,              /* e = 6 */
        4671183ULL,               /* e = 7 */
        152130ULL,                /* e = 8 */
        5445ULL,                  /* e = 9 */
        110ULL,                   /* e =10 */
        1ULL                      /* e =11 */
    };

    const __int128 denom = (__int128)39916800ULL;
    __int128 num = (__int128)163 * denom;   /* constant term*denom */
    __int128 pow = 1;                        /* n^e, starting with e = 0 */

    for (idx_t e = 1; e <= 11; ++e) {
        pow *= (__int128)n;                 /* pow = n^e */
        num += (__int128)coef[e] * pow;
    }

    return (seq_t)(num / denom);
}

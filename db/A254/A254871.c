/*
 * A254871  Seventh partial sums of fifth powers (A000584).
 * https://oeis.org/A254871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(n+5)*(n+6)*(n+7)* (-21 + 49*n
 *            + 56*n^2 + 14*n^3 + n^4)/3991680
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 19, u64 n <= 139, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* defined for n >= 1 */

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;

    seq_t poly = N4 + (seq_t)14 * N3 + (seq_t)56 * N2
                 + (seq_t)49 * N  - (seq_t)21;

    /* factors before cancelling the denominator */
    seq_t factors[9];
    factors[0] = N;          /* n       */
    factors[1] = N + 1;      /* n+1     */
    factors[2] = N + 2;      /* n+2     */
    factors[3] = N + 3;      /* n+3     */
    factors[4] = N + 4;      /* n+4     */
    factors[5] = N + 5;      /* n+5     */
    factors[6] = N + 6;      /* n+6     */
    factors[7] = N + 7;      /* n+7     */
    factors[8] = poly;       /* polynomial part */

    /* 3991680 = 2^7 * 3^4 * 5 * 7 * 11 */
    static const idx_t den_factors[14] = {
        2, 2, 2, 2, 2, 2, 2,  /* 2^7   */
        3, 3, 3, 3,           /* 3^4   */
        5, 7, 11              /* 5,7,11 */
    };

    idx_t k, j;
    for (k = 0; k < sizeof den_factors / sizeof den_factors[0]; ++k) {
        seq_t p = (seq_t)den_factors[k];
        for (j = 0; j < 9; ++j) {
            if (factors[j] % p == 0) {
                factors[j] /= p;
                break;
            }
        }
        /* Each prime factor must divide at least one remaining factor */
    }

    seq_t result = 1;
    for (j = 0; j < 9; ++j) {
        result *= factors[j];
    }

    return result;
}

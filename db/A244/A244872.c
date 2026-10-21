/*
 * A244872  Number of magic labelings with magic sum n of 4th graph shown in
 *          link.
 * https://oeis.org/A244872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*P + 45*(255 + (-1)^n)) / 11520 where P = Σ_{k=1}^7
 *            (coeff_k * n^k * (5760 / denom_k)) with coefficients and
 *            denominators from 17*n^7/1440 + 119*n^6/960 + 28*n^5/45 +
 *            721*n^4/384 + 5243*n^3/1440 + 721*n^2/160 + 129*n/40 +
 *            (-1)^n/256 + 255/256
 * Range:     u8 none, u16 n <= 0, u32 n <= 10, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* constants for the polynomial terms */
    static const unsigned long long coeff[8] = {
        0ULL,           /* unused index 0 */
        129ULL,  /* n^1 */
        721ULL,  /* n^2 */
        5243ULL, /* n^3 */
        721ULL,  /* n^4 */
        28ULL,   /* n^5 */
        119ULL,  /* n^6 */
        17ULL    /* n^7 */
    };
    static const unsigned long long mult[8] = {
        0ULL,   /* unused index 0 */
        144ULL, /* 5760/40   */
        36ULL,  /* 5760/160  */
        4ULL,   /* 5760/1440 */
        15ULL,  /* 5760/384  */
        128ULL, /* 5760/45   */
        6ULL,   /* 5760/960  */
        4ULL    /* 5760/1440 */
    };
    const seq_t LCM = (seq_t)5760;
    const seq_t DEN = (seq_t)11520;          /* lcm(5760,256)  */

    seq_t n_s = (seq_t)n;
    seq_t nPow[8];
    nPow[0] = (seq_t)1;
    for (idx_t i = 1; i <= 7; ++i)
        nPow[i] = nPow[i - 1] * n_s;

    seq_t P = (seq_t)0;
    for (idx_t k = 1; k <= 7; ++k)
        P += (seq_t)coeff[k] * nPow[k] * (seq_t)mult[k];

    seq_t R = ((n & 1ULL) == 0ULL) ? (seq_t)256 : (seq_t)254;
    seq_t sum_num = (seq_t)2 * P + (seq_t)45 * R;
    return sum_num / DEN;
}

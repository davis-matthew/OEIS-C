/*
 * A393925  Number of transitive relations on an n-set with exactly eight
 *          ordered pairs.
 * https://oeis.org/A393925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 429*C(n,4) + 23120*C(n,5) + 478635*C(n,6) +
 *            5507586*C(n,7) + 40073168*C(n,8) + 198343134*C(n,9) +
 *            696045600*C(n,10) + 1767981600*C(n,11) + 3257709840*C(n,12) +
 *            4276752480*C(n,13) + 3814050240*C(n,14) + 2075673600*C(n,15) +
 *            518918400*C(n,16)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 34, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    seq_t res = 1;
    idx_t i;
    for (i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (idx_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    static const seq_t coeff[13] = {
        429LL,
        23120LL,
        478635LL,
        5507586LL,
        40073168LL,
        198343134LL,
        696045600LL,
        1767981600LL,
        3257709840LL,
        4276752480LL,
        3814050240LL,
        2075673600LL,
        518918400LL
    };

    seq_t result = 0;
    idx_t k;
    for (k = 4; k <= 16; ++k) {
        if (n >= k) {
            seq_t bin = binomial(n, k);
            result += coeff[k - 4] * bin;
        }
    }
    return result;
}

/*
 * A395093  Number of transitive relations on an n-set with at most seven
 *          ordered pairs.
 * https://oeis.org/A395093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,0) + C(n,1) + 10*C(n,2) + 136*C(n,3) + 2565*C(n,4) +
 *            36981*C(n,5) + 350035*C(n,6) + 2200507*C(n,7) + 9502656*C(n,8) +
 *            28893816*C(n,9) + 62455680*C(n,10) + 94857840*C(n,11) +
 *            97130880*C(n,12) + 60540480*C(n,13) + 17297280*C(n,14)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 46, u128 n <= 1042,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_u64(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(n - k + i) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Coefficients for the polynomial expression, stored as unsigned
       integers to avoid truncation in tiny seq_t builds. */
    static const unsigned int coeff[15] = {
        1, 1, 10, 136, 2565, 36981, 350035,
        2200507, 9502656, 28893816, 62455680,
        94857840, 97130880, 60540480, 17297280
    };

    seq_t res = 0;
    for (idx_t k = 0; k <= 14 && k <= n; ++k) {
        seq_t b = binom_u64(n, k);          /* C(n,k) */
        res += (seq_t)coeff[k] * b;          /* coeff[k] * C(n,k) */
    }
    return res;
}

/*
 * A357938  Inverse Moebius transform of n * 2^omega(n).
 * https://oeis.org/A357938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} ( 2*(p^(e+1)-1)/(p-1)-1 ), a(1)=1
 * Range:     u8 n <= 16, u16 n <= 1404, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n = 0, guard only */
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* compute f = 2 * (p^(exp+1) - 1) / (p - 1) - 1 */
            seq_t pow_val = 1;
            for (idx_t i = 0; i < exp + 1; ++i)
                pow_val *= (seq_t)p;
            seq_t num = pow_val - 1;
            seq_t den = p - 1;
            seq_t f = 2 * (num / den) - 1;
            result *= f;
        }
    }
    if (m > 1) {
        /* remaining prime factor m (exponent 1) */
        idx_t exp = 1;
        seq_t pow_val = 1;
        for (idx_t i = 0; i < exp + 1; ++i)
            pow_val *= (seq_t)m;
        seq_t num = pow_val - 1;
        seq_t den = m - 1;
        seq_t f = 2 * (num / den) - 1;
        result *= f;
    }
    return result;
}

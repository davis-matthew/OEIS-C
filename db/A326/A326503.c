/*
 * A326503  Expansion of Sum_{k>=1} x^k * (1 - x^(2*k)) / (1 + x^k +
 *          x^(2*k))^2.
 * https://oeis.org/A326503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Sum_{d|n, d==1 (mod 3)} d - Sum_{d|n, d==2 (mod 3)} d
 * Range:     i8 n <= 12, i16 n <= 1092, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1)=1 */
    if (n == 0) return (seq_t)0; /* unused, for safety */
    if (n == 1) return (seq_t)1;

    seq_t result = 1;
    idx_t nn = n;            /* working copy of n */
    /* Trial‑division up to sqrt(nn) */
    for (idx_t p = 2; p <= nn / p; ++p) {
        if (nn % p == 0) {
            idx_t e = 0;
            while (nn % p == 0) { nn /= p; ++e; }

            seq_t val;
            if (p == 3) {
                val = 1;
            } else if (p % 3 == 1) {
                seq_t base = (seq_t)p;
                seq_t pw = 1;
                for (idx_t i = 0; i < e + 1; ++i) pw *= base;
                val = (pw - 1) / (base - 1);
            } else { /* p % 3 == 2 */
                seq_t base = - (seq_t)p;
                seq_t pw = 1;
                for (idx_t i = 0; i < e + 1; ++i) pw *= base;
                seq_t denom = - (seq_t)p - 1;
                val = (pw - 1) / denom;
            }
            result *= val;
        }
    }
    /* Remaining prime factor (if any) */
    if (nn > 1) {
        idx_t p = nn;            /* exponent is 1 */
        seq_t val;
        if (p == 3) {
            val = 1;
        } else if (p % 3 == 1) {
            seq_t base = (seq_t)p;
            seq_t pw = base * base;          /* e+1 = 2 */
            val = (pw - 1) / (base - 1);
        } else { /* p % 3 == 2 */
            seq_t base = - (seq_t)p;
            seq_t pw = base * base;          /* e+1 = 2 */
            seq_t denom = - (seq_t)p - 1;
            val = (pw - 1) / denom;
        }
        result *= val;
    }
    return result;
}

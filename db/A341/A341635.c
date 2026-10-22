/*
 * A341635  a(n) = Sum_{d|n} phi(d) * mu(d) * mu(n/d).
 * https://oeis.org/A341635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Σ_{d|n} φ(d)·μ(d)·μ(n/d) (equivalently, a is
 *            multiplicative with a(p) = -p, a(p²) = p-1, a(pᵉ) = 0 for e ≥ 3)
 * Range:     i8 n <= 128, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;              /* a(1)=1 */

    seq_t result = 1;
    idx_t m = n;

    /* handle factor 2 */
    idx_t exp = 0;
    while ((m & 1) == 0) {             /* test divisibility by 2 */
        m >>= 1;
        exp++;
    }
    if (exp) {
        if (exp == 1) result *= (seq_t)(-2);
        else if (exp == 2) result *= 1;   /* 2-1 = 1, no change */
        else return 0;                    /* exponent ≥3 → zero */
    }

    /* handle odd prime factors */
    for (idx_t i = 3; i <= m / i; i += 2) {
        if (m % i == 0) {
            exp = 0;
            while (m % i == 0) {
                m /= i;
                exp++;
            }
            if (exp == 1) result *= (seq_t)(-(seq_t)i);
            else if (exp == 2) result *= (seq_t)((seq_t)i - 1);
            else return 0;                /* exponent ≥3 → zero */
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        result *= (seq_t)(-(seq_t)m);
    }

    return result;
}

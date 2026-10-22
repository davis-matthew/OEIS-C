/*
 * A363628  Expansion of Sum_{k>0} (1/(1-x^k)^3 - 1).
 * https://oeis.org/A363628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n} binomial(d+2,2) = (σ₂(n) + 3σ₁(n) + 2τ(n)) / 2
 *            where σ₁ is the sum of divisors, σ₂ the sum of squared divisors,
 *            and τ the divisor count. The function factors n and evaluates
 *            the multiplicative formulas for σ₁, σ₂, and τ
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* safeguard for indices < offset */

    idx_t temp = n;
    seq_t sigma1 = (seq_t)1;
    seq_t sigma2 = (seq_t)1;
    seq_t tau    = (seq_t)1;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t pseq = (seq_t)p;

            /* sigma1 component: (p^{e+1} - 1) / (p - 1) */
            seq_t pow_pe1 = (seq_t)1;
            for (int i = 0; i < e + 1; ++i) pow_pe1 *= pseq;
            seq_t term1 = (pow_pe1 - (seq_t)1) / (pseq - (seq_t)1);
            sigma1 *= term1;

            /* sigma2 component: (p^{2e+2} - 1) / (p² - 1) */
            seq_t pow_p2e2 = (seq_t)1;
            for (int i = 0; i < 2 * e + 2; ++i) pow_p2e2 *= pseq;
            seq_t term2 = (pow_p2e2 - (seq_t)1) / ((pseq * pseq) - (seq_t)1);
            sigma2 *= term2;

            /* tau component: e + 1 */
            tau *= (seq_t)(e + 1);
        }
    }

    /* Any remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        seq_t pseq = (seq_t)temp;
        /* exponent e = 1 */
        /* sigma1 : (p² - 1) / (p - 1) = p + 1 */
        seq_t pow_pe1 = pseq * pseq;           /* p² */
        seq_t term1   = (pow_pe1 - (seq_t)1) / (pseq - (seq_t)1);
        sigma1 *= term1;

        /* sigma2 : (p⁴ - 1) / (p² - 1) = p² + 1 */
        seq_t pow_p2e2 = pseq * pseq * pseq * pseq; /* p⁴ */
        seq_t term2   = (pow_p2e2 - (seq_t)1) / ((pseq * pseq) - (seq_t)1);
        sigma2 *= term2;

        tau *= (seq_t)2;   /* e + 1 = 2 */
    }

    seq_t result = (sigma2 + (seq_t)3 * sigma1 + (seq_t)2 * tau) / (seq_t)2;
    return result;
}

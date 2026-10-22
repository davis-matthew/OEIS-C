/*
 * A353752  Multiplicative with a(p^e) = phi(sigma(p^e)).
 * https://oeis.org/A353752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} φ(σ(p^e)), where σ(p^e) = (p^(e+1)–1)/(p–1)
 * Range:     u8 n <= 143, u16 n <= 9583, u32 n <= 9583, u64 n <= 9583,
 *            u128 n <= 9583, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative sequence A353752 */

#include <stdint.h>
#include <stddef.h>

/* Helper: compute φ(m) for 64‑bit integers, return as seq_t */
static seq_t phi_uint64(uint64_t m)
{
    if (m <= 1) return (seq_t)m;          /* φ(0)=0, φ(1)=1 */
    uint64_t result = m;
    uint64_t temp = m;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1) result -= result / temp;   /* remaining prime factor */
    return (seq_t)result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* a(0) is undefined in OEIS entry */
    if (n == 1) return (seq_t)1;  /* empty product */

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;     /* copy for factorisation */

    while (m > 1) {
        /* Find smallest prime factor by trial division */
        uint32_t p = 2;
        while (p * (uint64_t)p <= m && m % p != 0) ++p;
        if (p * (uint64_t)p > m) p = (uint32_t)m;  /* m is prime */

        /* Get exponent e of this prime factor */
        unsigned e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* Compute σ(p^e) = (p^(e+1) – 1)/(p – 1) using 64‑bit arithmetic */
        uint64_t pow = 1;
        for (unsigned i = 0; i < e + 1; ++i) pow *= (uint64_t)p;
        uint64_t sigma = (pow - 1) / ((uint64_t)p - 1);

        /* φ(σ(p^e)) */
        seq_t phis = phi_uint64(sigma);

        result *= phis;
    }

    return result;
}

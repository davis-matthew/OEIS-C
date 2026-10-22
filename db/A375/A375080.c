/*
 * A375080  a(n) is the numerator of ( Sum_{d|n} (n - d) )/tau(n).
 * https://oeis.org/A375080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator((n - sigma(n))/tau(n))
 * Range:     u8 n <= 47, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/seq_gcd.h"

/* local helper: compute base^exp, exp < 2^64 */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= base;
    return r;
}

seq_t a(idx_t n)
{
    /* n = 1 → σ(1)=1, τ(1)=1 → (1‑1)/1 = 0 */
    if (n == 1) return 0;

    /* factorisation of n */
    uint64_t m = n;          /* we only need to trial‑divide up to sqrt(m) */
    seq_t tau = 1;           /* product of (e+1) over all prime factors */
    seq_t sigma = 1;         /* product of σ(p^e) over all prime factors */

    /* handle factor 2 */
    if ((m & 1ULL) == 0) {
        idx_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        tau *= (e + 1);
        seq_t p = 2;
        seq_t part = pow_seq(p, e + 1) - 1;          /* p^(e+1)-1   */
        part = part / (p - 1);                      /* divide by p‑1 */
        sigma *= part;
    }

    /* trial division by odd integers */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            tau *= (e + 1);
            seq_t pp = (seq_t)p;
            seq_t part = pow_seq(pp, e + 1) - 1;      /* p^(e+1)-1   */
            part = part / (pp - 1);                  /* divide by p‑1 */
            sigma *= part;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1) {
        seq_t pp = (seq_t)m;
        tau *= 2;                     /* exponent 1 → (1+1) */
        sigma *= (pp + 1);             /* σ(p) = p+1   */
    }

    /* term = n*tau - sigma   */
    seq_t term = (seq_t)n * tau - sigma;

    /* reduce the fraction term / tau */
    seq_t g = seq_gcd(term, tau);
    return term / g;
}

/*
 * A366135  Expansion of Sum_{k>=1} k^3 * x^k/(1 - x^k)^3.
 * https://oeis.org/A366135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (n * sigma(n) + sigma2(n)) / 2
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A366135. */

#include <stdint.h>

typedef struct {
    uint64_t p;  /* prime factor */
    int e;       /* exponent */
} factor_t;

/* Recursive divisor generation: accumulates sigma and sigma2. */
static void _generate_divisors(int idx, seq_t cur, int cnt,
                               factor_t *fac, seq_t *sigma, seq_t *sigma2)
{
    if (idx == cnt) {
        *sigma += cur;
        *sigma2 += cur * cur;
        return;
    }
    seq_t c = 1;
    for (int e = 0; e <= fac[idx].e; ++e) {
        _generate_divisors(idx + 1, cur * c, cnt, fac, sigma, sigma2);
        c *= (seq_t)fac[idx].p;
    }
}

/* Main function: a(n) = n * (n * sigma(n) + sigma2(n)) / 2. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for off‑by‑one calls */

    /* Factorize n using trial division (n ≤ ~10⁴ for intended use). */
    uint64_t temp = n;
    factor_t fac[64];
    int cnt = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            fac[cnt].p = p;
            fac[cnt].e = e;
            ++cnt;
        }
    }
    if (temp > 1) {                     /* remaining prime factor */
        fac[cnt].p = temp;
        fac[cnt].e = 1;
        ++cnt;
    }

    /* Accumulate sums of divisors and squared divisors. */
    seq_t sigma  = 0;
    seq_t sigma2 = 0;
    _generate_divisors(0, 1, cnt, fac, &sigma, &sigma2);

    /* Final formula.  All intermediate arithmetic uses seq_t. */
    seq_t nn  = (seq_t)n;
    seq_t res = nn * ((nn * sigma) + sigma2) / 2;
    return res;
}

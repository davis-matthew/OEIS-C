/*
 * A323600  Dirichlet convolution of sigma with omega.
 * https://oeis.org/A323600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} sigma(d)*omega(n/d)
 * Range:     u8 n <= 59, u16 n <= 6479, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void dfs_div(idx_t pos, seq_t cur_sigma, unsigned int omega_nd,
                    const uint64_t *primes, const idx_t *exps, idx_t num, seq_t *sum)
{
    if (pos == num) {
        /* the co‑divisor has omega_nd distinct prime factors */
        *sum += cur_sigma * (seq_t)omega_nd;
        return;
    }
    idx_t e = exps[pos];

    /* pre‑compute sigma(p^k) for this prime (k = 0..e) */
    seq_t sigma_vals[64];           /* 64 is ample for e ≤ log₂(10⁴) */
    seq_t pow = 1;
    sigma_vals[0] = 1;              /* sigma(p⁰) */
    for (idx_t k = 1; k <= e; ++k) {
        pow *= (seq_t)primes[pos];
        sigma_vals[k] = sigma_vals[k - 1] + pow;   /* sum_{i=0}^k p^i */
    }

    for (idx_t k = 0; k <= e; ++k) {
        seq_t new_sigma = cur_sigma * sigma_vals[k];
        unsigned int new_omega = omega_nd + (k < e ? 1 : 0); /* prime in co‑divisor */
        dfs_div(pos + 1, new_sigma, new_omega, primes, exps, num, sum);
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;   /* sigma(1)=1, omega(1)=0 */

    uint64_t temp = (uint64_t)n;

    uint64_t primes[20];
    idx_t exps[20];
    idx_t num = 0;

    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            primes[num]   = p;
            exps[num]     = cnt;
            ++num;
        }
    }
    if (temp > 1) {
        primes[num] = temp;
        exps[num]   = 1;
        ++num;
    }

    seq_t result = 0;
    dfs_div(0, (seq_t)1, 0, primes, exps, num, &result);
    return result;
}

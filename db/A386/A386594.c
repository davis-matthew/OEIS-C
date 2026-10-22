/*
 * A386594  a(n) = Sum_{d|n} d * c(n/d), where c = A351114.
 * https://oeis.org/A386594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d) if φ(d) | σ(d), else 0
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* recursive helper to enumerate all divisors of n using its prime factorisation */
static void dfs(idx_t n, int idx, idx_t d, seq_t phi, seq_t sigma,
                seq_t *res, int nf, const idx_t *primes, const int *exps)
{
    if (idx == nf) {
        if (sigma % phi == 0)
            *res += (seq_t)n / d;
        return;
    }
    idx_t p = primes[idx];
    int e = exps[idx];
    idx_t pow_idx = 1;
    seq_t phi_part = 1;
    seq_t sigma_part = 1;
    for (int k = 0; k <= e; ++k) {
        idx_t new_d = d * pow_idx;
        seq_t new_phi = phi * phi_part;
        seq_t new_sigma = sigma * sigma_part;
        dfs(n, idx + 1, new_d, new_phi, new_sigma, res, nf, primes, exps);
        if (k == e) break;
        pow_idx *= p;
        if (k == 0) {
            phi_part = (seq_t)(p - 1);
            sigma_part = 1 + (seq_t)p;
        } else {
            phi_part *= (seq_t)p;
            sigma_part += pow_idx;
        }
    }
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* factorise n */
    idx_t temp = n;
    idx_t primes[16];
    int exps[16];
    int nf = 0;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            primes[nf] = p;
            exps[nf] = cnt;
            ++nf;
        }
    }
    if (temp > 1) {
        primes[nf] = temp;
        exps[nf] = 1;
        ++nf;
    }
    dfs(n, 0, 1, 1, 1, &res, nf, primes, exps);
    return res;
}

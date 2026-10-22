/*
 * A365687  a(n) = number of fractions m/n, 0 <= m < n, gcd(m,n) = 1 whose
 *          partial fraction decomposition has integer part 0.
 * https://oeis.org/A365687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced fractions m/n (0 ≤ m < n) whose partial
 *            fraction decomposition has integer part 0. For a prime‑power
 *            denominator a(n) = φ(n); for a product of powers of two distinct
 *            primes a(n) = φ(n)/2; for non‑hyperbolic numbers a(n) = 0;
 *            otherwise count the tuples (a₁,…,aₖ) of coprime...
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/phi.h"

static void dfs_count(size_t idx, uint64_t current, seq_t *cnt,
                      uint64_t n, const uint64_t *primes,
                      const uint64_t *ds, const uint64_t *bs, size_t k)
{
    if (idx == k) {
        (*cnt)++;
        return;
    }
    uint64_t prime = primes[idx];
    uint64_t d = ds[idx];
    uint64_t b = bs[idx];
    for (uint64_t a = 1; a < d; ++a) {
        if (a % prime == 0)
            continue;
        uint64_t newSum = current + a * b;
        if (newSum >= n)
            continue;
        dfs_count(idx + 1, newSum, cnt, n, primes, ds, bs, k);
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t n64 = (uint64_t)n;

    /* factor n */
    uint64_t primes[64];
    uint32_t exps[64];
    uint64_t ds[64];   /* prime powers p^r */
    uint64_t bs[64];   /* n / p^r */
    size_t k = 0;

    uint64_t tmp = n64;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint32_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            primes[k] = p;
            exps[k] = e;
            uint64_t d = 1;
            for (uint32_t t = 0; t < e; ++t) d *= p;
            ds[k] = d;
            bs[k] = n64 / d;
            ++k;
        }
    }
    if (tmp > 1) {
        primes[k] = tmp;
        exps[k] = 1;
        ds[k] = tmp;
        bs[k] = n64 / tmp;
        ++k;
    }

    /* non‑hyperbolic check: Σ n/d_i ≥ n  ⇔  Σ 1/p_i^r_i ≥ 1 */
    uint64_t sumRecip = 0;
    for (size_t i = 0; i < k; ++i)
        sumRecip += bs[i];
    if (sumRecip >= n64)
        return (seq_t)0;

    if (k == 1)
        return phi(n);

    if (k == 2) {
        seq_t phi_n = phi(n);
        return phi_n / (seq_t)2;
    }

    /* k ≥ 3: brute‑force enumeration of the tuples */
    seq_t count = 0;
    dfs_count(0, 0, &count, n64, primes, ds, bs, k);
    return count;
}

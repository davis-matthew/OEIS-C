/*
 * A318875  Number of divisors d of n for which 2*phi(d) < d.
 * https://oeis.org/A318875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that 2*phi(d) < d
 * Range:     u8 n <= 481, u16 n <= 9796, u32 n <= 9796, u64 n <= 9796,
 *            u128 n <= 9796, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Helper: factor n into primes and exponents.  primes[] and exps[] must
 * have space for at least 64 entries.  Returns the number of distinct
 * prime factors.  The caller must ensure that n >= 1. */
static int factor_n(uint64_t n, uint32_t *primes, uint32_t *exps)
{
    int cnt = 0;
    if (n % 2 == 0) {
        uint32_t c = 0;
        while (n % 2 == 0) {
            n /= 2;
            ++c;
        }
        primes[cnt] = 2;
        exps[cnt] = c;
        ++cnt;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint32_t c = 0;
            while (n % p == 0) {
                n /= p;
                ++c;
            }
            primes[cnt] = (uint32_t)p;
            exps[cnt] = c;
            ++cnt;
        }
    }
    if (n > 1) {
        /* remaining prime factor */
        primes[cnt] = (uint32_t)n;
        exps[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* Recursive enumeration of divisors.  For each divisor d, we compute
 * its phi(d) from the factorizations already traversed and test the
 * condition 2*phi(d) < d.  The result is accumulated in *cnt. */
static void dfs(const uint32_t *primes, const uint32_t *exps,
                int idx, int nf, seq_t d, seq_t phi, seq_t *cnt)
{
    if (idx == nf) {
        /* finished one divisor */
        seq_t two_phi = phi + phi;
        if (two_phi < d) {
            (*cnt)++;
        }
        return;
    }

    uint32_t p = primes[idx];
    uint32_t e = exps[idx];

    /* precompute powers of p up to e */
    seq_t powp[65];   /* e <= 64 */
    powp[0] = 1;
    for (uint32_t i = 1; i <= e; ++i) {
        powp[i] = powp[i - 1] * (seq_t)p;
    }

    for (uint32_t k = 0; k <= e; ++k) {
        seq_t nd = d * powp[k];
        seq_t phi_part;
        if (k == 0) {
            phi_part = 1;
        } else {
            /* phi part for exponent k: p^(k-1)*(p-1) */
            phi_part = powp[k - 1] * (seq_t)(p - 1);
        }
        seq_t nphi = phi * phi_part;
        dfs(primes, exps, idx + 1, nf, nd, nphi, cnt);
    }
}

/* Main function requested by the problem */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    /* Factor n */
    uint32_t primes[64];
    uint32_t exps[64];
    int nf = factor_n((uint64_t)n, primes, exps);

    seq_t count = 0;
    dfs(primes, exps, 0, nf, (seq_t)1, (seq_t)1, &count);
    return count;
}

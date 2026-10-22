/*
 * A321519  Let d(n,i), i = 1..k be the k divisors of n^2 + 1 (the number 1 is
 *          not counted). a(n) is the number of ordered pairs d(n,i) < d(n,j)
 *          such that gcd(d(n,i), d(n,j)) = 1.
 * https://oeis.org/A321519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((S-1)/2) - (τ(n²+1)-1) where S = Σ_{d|m} μ(d)·τ(m/d)²
 *            for m = n²+1 and τ(k) is the number of divisors of k. This
 *            equals A089233(m) minus the pairs involving the divisor 1.
 *            Implementation factors m, enumerates all its divisors using a
 *            depth‑first search, and accumulates the required sum. R...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static void dfs_divisors(int idx, const uint64_t *exps, size_t cnt,
                         int mu, uint64_t tau_sub, uint64_t *S)
{
    if (idx == (int)cnt) {
        if (mu != 0) {
            /* add mu * tau_sub² to the sum */
            *S += (uint64_t)mu * (tau_sub * tau_sub);
        }
        return;
    }

    /* for exponent a = 0 .. exps[idx] */
    for (uint64_t a = 0; a <= exps[idx]; ++a) {
        int mu_next = mu;
        if (a >= 2) {
            mu_next = 0;          /* μ becomes zero once a prime power ≥ p² divides d */
        } else if (a == 1) {
            mu_next = -mu_next;   /* flip sign for the extra prime factor */
        }

        uint64_t tau_next = tau_sub * (exps[idx] - a + 1);
        if (mu_next) { /* skip branches that will contribute zero */
            dfs_divisors(idx + 1, exps, cnt, mu_next, tau_next, S);
        }
    }
}

seq_t a(idx_t n)
{
    /* compute m = n² + 1 using 64‑bit arithmetic */
    uint64_t nn = (uint64_t)n;
    uint64_t m  = nn * nn + 1;

    /* factorize m */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* compute τ(m) */
    uint64_t tau_m = 1;
    for (size_t i = 0; i < cnt; ++i) {
        tau_m *= (exps[i] + 1);
    }

    /* enumerate all divisors and compute S */
    uint64_t S = 0;
    dfs_divisors(0, exps, cnt, 1, 1, &S);

    /* a(n) = ((S-1)/2) - (τ(m)-1) */
    int64_t  temp = (int64_t)(S - 1) / 2 - (int64_t)(tau_m - 1);
    /* cast to seq_t */
    return (seq_t)temp;
}

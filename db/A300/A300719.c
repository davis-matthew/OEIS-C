/*
 * A300719  Difference between A003557 (n divided by largest squarefree
 *          divisor of n) and its Möbius transform.
 * https://oeis.org/A300719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003557(n) - Sum_{d|n} mu(n/d)*A003557(d)
 * Range:     u8 n <= 548, u16 n <= 7230, u32 n <= 7230, u64 n <= 7230,
 *            u128 n <= 7230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the sequence a(n) as defined for OEIS A300719. */
/* It uses only integer arithmetic and the provided seq_t type. */

#include <stdint.h>
#include <stdbool.h>

/* Factorises n into distinct prime factors and their exponents.   */
static void factorize(uint64_t n, uint64_t primes[], int exps[], int *cnt) {
    int c = 0;
    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[c] = p;
            exps[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        primes[c] = n;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Recursively enumerates all divisors of n to accumulate sums.   */
static void dfs(const uint64_t primes[], const int exps[], int cnt, int idx,
                seq_t g_val, int mu_sign, bool zero_mu,
                seq_t *sumPos, seq_t *sumNeg) {
    if (idx == cnt) {
        if (zero_mu) return;
        if (mu_sign == 1)
            *sumPos += g_val;
        else
            *sumNeg += g_val;
        return;
    }

    uint64_t p = primes[idx];
    int e = exps[idx];
    /* Pre‑compute powers of p up to e.                         */
    seq_t pow_arr[65];
    pow_arr[0] = 1;
    for (int i = 1; i <= e; ++i)
        pow_arr[i] = pow_arr[i - 1] * p;

    for (int f = 0; f <= e; ++f) {
        bool nz = zero_mu;
        int ms = mu_sign;
        int rem = e - f;
        if (rem > 1)
            nz = true;               /* mu = 0 for squared factor */
        else if (rem == 1)
            ms = -mu_sign;           /* flip sign when exactly one prime in n/d */

        seq_t newG = g_val;
        if (f > 0)
            newG *= pow_arr[f - 1];   /* multiply by p^(f-1) for g(d) */

        dfs(primes, exps, cnt, idx + 1, newG, ms, nz, sumPos, sumNeg);
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* undefined for 0 in OEIS; return 0 for safety */

    uint64_t N = (uint64_t)n;

    uint64_t primes[64];
    int exps[64];
    int cnt = 0;
    factorize(N, primes, exps, &cnt);

    /* Compute g(n) = n / rad(n) */
    seq_t rad = 1;
    for (int i = 0; i < cnt; ++i)
        rad *= primes[i];
    seq_t g_n = (seq_t)N / rad;

    seq_t sumPos = 0;
    seq_t sumNeg = 0;
    dfs(primes, exps, cnt, 0, 1, 1, false, &sumPos, &sumNeg);

    seq_t totalSum;
    if (sumPos >= sumNeg)
        totalSum = sumPos - sumNeg;
    else
        totalSum = 0;   /* should not happen for valid n */

    seq_t result = g_n - totalSum;
    return result;
}

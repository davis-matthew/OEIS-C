/*
 * A329647  Bitwise-XOR of exponents of prime factors of A108951(n), where
 *          A108951 is fully multiplicative with a(prime(i)) = prime(i)# =
 *          Product_{i=1..i} A000040(i).
 * https://oeis.org/A329647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 7509 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* With n = prod p_{i}^{e_i}, the exponent of prime(j) in A108951(n) is
 * S_j = sum of e_i over i >= j, and a(n) = XOR of S_j, j = 1..max index.
 * S_j is constant between consecutive prime indices i_1 < i_2 < ... of n:
 * S_j = e_t + e_{t+1} + ... for i_{t-1} < j <= i_t, i.e. i_t - i_{t-1}
 * times, and XOR-ing a value an even number of times cancels it. pi comes
 * from a sieve that grows with n (the original had fixed tables for
 * n <= 10000). */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *pi_tab = NULL;           /* pi_tab[x] = pi(x), x <= pi_lim */
static uint64_t pi_lim = 0;

static void ensure_pi(uint64_t n)
{
    if (n <= pi_lim) return;
    uint64_t lim = pi_lim ? 2 * pi_lim : 1024;
    while (lim < n) lim *= 2;
    unsigned char *comp = (unsigned char *)calloc(lim + 1, 1);
    uint32_t *t = (uint32_t *)malloc((lim + 1) * sizeof *t);
    if (!comp || !t) abort();
    for (uint64_t i = 2; i * i <= lim; ++i)
        if (!comp[i])
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
    uint32_t c = 0;
    for (uint64_t i = 0; i <= lim; ++i) {
        if (i >= 2 && !comp[i]) ++c;
        t[i] = c;
    }
    free(comp);
    free(pi_tab);
    pi_tab = t;
    pi_lim = lim;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    ensure_pi((uint64_t)n);
    uint64_t idx[64], e[64], m = (uint64_t)n, total = 0;
    int k = 0;
    for (uint64_t p = 2; p * p <= m; ++p)      /* increasing primes, so increasing indices */
        if (m % p == 0) {
            idx[k] = pi_tab[p];
            e[k] = 0;
            while (m % p == 0) { m /= p; ++e[k]; }
            total += e[k++];
        }
    if (m > 1) { idx[k] = pi_tab[m]; e[k] = 1; total += 1; ++k; }

    uint64_t x = 0, prev = 0, S = total;
    for (int t = 0; t < k; ++t) {
        if ((idx[t] - prev) & 1) x ^= S;
        prev = idx[t];
        S -= e[t];
    }
    return (seq_t)x;
}

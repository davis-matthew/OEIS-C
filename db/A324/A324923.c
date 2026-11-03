/*
 * A324923  Number of distinct factors in the factorization of n into factors
 *          q(i) = prime(i)/i, i > 0.
 * https://oeis.org/A324923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of distinct factors in the factorization of n into
 *            factors q(i) = prime(i)/i, i>0. This implementation computes the
 *            unique representation of n in terms of the q(i) and counts how
 *            many distinct indices i occur. It works for any n ≥ 1 and uses
 *            only integer arithmetic
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 124 terms through n = 8771 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n = prod q(i)^e_i with q(i) = prime(i)/i is found greedily from the top:
   the largest prime p | n, to the power e, must come from q(pi(p))^e, which
   also divides out i^e, i = pi(p) < p; so take it, multiply what is left by
   i^e and repeat. a(n) is the number of distinct i used. The prime powers
   are kept as a short list and pi comes from a sieve that grows with n (the
   original had fixed tables for n <= 20000). */

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

/* the prime powers still to place: P[0..k) with exponents E */
static uint64_t P[64], E[64];
static int k;

static void add(uint64_t p, uint64_t e)
{
    for (int j = 0; j < k; ++j)
        if (P[j] == p) { E[j] += e; return; }
    P[k] = p;
    E[k] = e;
    ++k;
}

static void add_factors(uint64_t m, uint64_t mult)
{
    for (uint64_t p = 2; p * p <= m; ++p)
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            add(p, e * mult);
        }
    if (m > 1) add(m, mult);
}

seq_t a(idx_t n)
{
    ensure_pi((uint64_t)n);
    k = 0;
    add_factors((uint64_t)n, 1);
    seq_t result = 0;                      /* each i is used once: the primes taken decrease */
    while (k > 0) {
        int top = 0;
        for (int j = 1; j < k; ++j)
            if (P[j] > P[top]) top = j;
        uint64_t p = P[top], e = E[top];
        P[top] = P[k - 1];
        E[top] = E[k - 1];
        --k;
        ++result;
        add_factors(pi_tab[p], e);
    }
    return result;
}

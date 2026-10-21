/*
 * A249442  a(n) is the smallest m such that binomial(n,m) is not squarefree,
 *          or a(n)=0, if there is no such m.
 * https://oeis.org/A249442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m (0 ≤ m ≤ n) such that binomial(n,m) is not
 *            squarefree; a(n) = 0 if no such m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static idx_t g_maxN = 0;                /* maximum n for which sieve has been built */
static uint16_t *g_spf = NULL;          /* smallest prime factor array */
static uint16_t *g_primeIdx = NULL;     /* mapping prime → index+1 in primes list */
static idx_t *g_primes = NULL;          /* list of primes */
static idx_t g_primeCount = 0;          /* number of primes ≤ g_maxN */

static int *g_exponents = NULL;         /* exponents of primes in current binomial */
static idx_t g_expLen = 0;              /* allocated length of exponents array */

/* Ensure that primes and spf arrays cover all integers up to n. */
static void ensure_sieve(idx_t n)
{
    if (n <= g_maxN) return;

    /* free old structures if any */
    free(g_spf);
    free(g_primeIdx);
    free(g_primes);

    /* allocate spf array */
    g_spf = (uint16_t *)malloc((n + 1) * sizeof(uint16_t));
    for (idx_t i = 0; i <= n; ++i) g_spf[i] = (uint16_t)i;

    /* sieve for smallest prime factor */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (g_spf[i] == i) {
            for (idx_t j = i * i; j <= n; j += i) {
                if (g_spf[j] == j) g_spf[j] = (uint16_t)i;
            }
        }
    }

    /* build primes list */
    g_primeCount = 0;
    for (idx_t i = 2; i <= n; ++i) if (g_spf[i] == i) ++g_primeCount;
    g_primes = (idx_t *)malloc(g_primeCount * sizeof(idx_t));
    idx_t pos = 0;
    for (idx_t i = 2; i <= n; ++i) if (g_spf[i] == i) g_primes[pos++] = i;

    /* map prime → index+1 (0 indicates non‑prime) */
    g_primeIdx = (uint16_t *)malloc((n + 1) * sizeof(uint16_t));
    memset(g_primeIdx, 0, (n + 1) * sizeof(uint16_t));
    for (idx_t i = 0; i < g_primeCount; ++i) g_primeIdx[g_primes[i]] = (uint16_t)(i + 1);

    g_maxN = n;
}

/* Update exponent counts for factor x.
   sign = +1 for multiplication, -1 for division. */
static void update_factor(uint64_t x, int sign,
                          uint16_t *primeIdx,
                          int *exponents,
                          idx_t primeCount,
                          idx_t *bad)
{
    while (x > 1) {
        uint16_t p = g_spf[(uint32_t)x];
        uint32_t cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
        idx_t idx = primeIdx[p] - 1;  /* primeIdx gives 0 for non‑prime */
        int before = exponents[idx];
        int after = before + sign * cnt;
        exponents[idx] = after;

        if (before < 2 && after >= 2) ++(*bad);
        else if (before >= 2 && after < 2) --(*bad);
    }
}

seq_t a(idx_t n)
{
    /* Since binomial(0,0)=1 and all binomials for n=0,1,2,3 are squarefree,
       we can early exit for these very small n. */
    if (n <= 3) return 0;

    ensure_sieve(n);

    /* Ensure exponents array covers all primes ≤ n */
    if (g_expLen < g_primeCount) {
        free(g_exponents);
        g_exponents = (int *)malloc(g_primeCount * sizeof(int));
        g_expLen = g_primeCount;
    }
    memset(g_exponents, 0, g_primeCount * sizeof(int));

    idx_t bad = 0;    /* number of primes with exponent ≥ 2 */

    for (idx_t m = 1; m <= n; ++m) {
        uint64_t mult = (uint64_t)(n - (m - 1));
        uint64_t divide = (uint64_t)m;

        update_factor(mult, +1, g_primeIdx, g_exponents, g_primeCount, &bad);
        update_factor(divide, -1, g_primeIdx, g_exponents, g_primeCount, &bad);

        if (bad > 0) return (seq_t)m;
    }

    return 0;
}

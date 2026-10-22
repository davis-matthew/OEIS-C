/*
 * A332461  a(n) = Product_{d|n, d>1} A000040(A297113(d)), where A000040(n)
 *          gives the n-th prime, and A297113(n) = the excess of n plus the
 *          index of the largest dividing prime (A046660 + A061395).
 * https://oeis.org/A332461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d>1} A000040(A297113(d))
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 119, u64 n <= 479,
 *            u128 n <= 2519, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* static tables used for on‑demand precomputation */
static uint32_t *g_spf           = NULL;   /* smallest prime factor */
static uint8_t  *g_omega_tot     = NULL;   /* total prime multiplicity Ω */
static uint8_t  *g_omega_dist    = NULL;   /* distinct primes ω */
static uint32_t *g_lpf           = NULL;   /* largest prime factor */
static uint32_t *g_primes        = NULL;   /* list of primes (0‑based) */
static uint32_t *g_prime_idx     = NULL;   /* index of prime → 1‑based */
static size_t    g_prime_cnt     = 0;
static idx_t     g_limit         = 0;       /* current precomputation limit */

/* ------------------------------------------------------------ */
static void ensure_precompute(idx_t n)
{
    if (n <= 1) return;                    /* a(1) needs no data */

    /* choose a safe upper bound for primes needed when n≤limit */
    idx_t limit = n * 2 + 100;

    if (limit <= g_limit) return;          /* already enough */

    /* free any previous allocations */
    free((void*)g_spf);
    free((void*)g_omega_tot);
    free((void*)g_omega_dist);
    free((void*)g_lpf);
    free((void*)g_primes);
    free((void*)g_prime_idx);

    /* allocate new tables */
    g_spf        = (uint32_t*)calloc((size_t)limit + 1, sizeof(uint32_t));
    g_omega_tot  = (uint8_t*)calloc((size_t)limit + 1, sizeof(uint8_t));
    g_omega_dist = (uint8_t*)calloc((size_t)limit + 1, sizeof(uint8_t));
    g_lpf        = (uint32_t*)calloc((size_t)limit + 1, sizeof(uint32_t));
    g_primes     = (uint32_t*)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    g_prime_idx  = (uint32_t*)calloc((size_t)limit + 1, sizeof(uint32_t));

    /* linear sieve for prime factorisation data */
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (g_spf[i] == 0) {
            g_spf[i] = (uint32_t)i;
            g_primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt &&
                         (idx_t)g_primes[j] <= g_spf[i] &&
                         i * (idx_t)g_primes[j] <= limit; ++j) {
            g_spf[i * (idx_t)g_primes[j]] = g_primes[j];
        }
    }
    g_prime_cnt = primes_cnt;

    /* precompute Ω, ω and LPF */
    g_omega_tot[1]  = 0;
    g_omega_dist[1] = 0;
    g_lpf[1]        = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        uint32_t p = g_spf[i];
        idx_t j    = i / p;
        g_omega_tot[i] = g_omega_tot[j] + 1;
        if (g_spf[j] == p)
            g_omega_dist[i] = g_omega_dist[j];
        else
            g_omega_dist[i] = g_omega_dist[j] + 1;
        g_lpf[i] = (p > g_lpf[j]) ? p : g_lpf[j];
    }

    /* map prime → 1‑based index */
    for (size_t k = 0; k < g_prime_cnt; ++k)
        g_prime_idx[g_primes[k]] = (uint32_t)(k + 1);

    g_limit = limit;
}

/* ------------------------------------------------------------ */
static void gen_divs(int idx,
                     uint64_t prod,
                     seq_t *res,
                     const uint32_t *pf,
                     const uint32_t *exp,
                     int tot)
{
    if (idx == tot) {
        if (prod > 1) {
            uint32_t lpf_val   = g_lpf[(idx_t)prod];
            uint32_t lpf_index = g_prime_idx[lpf_val];     /* 1‑based */
            uint8_t  excess    = g_omega_tot[(idx_t)prod] -
                                 g_omega_dist[(idx_t)prod];
            uint32_t final_idx = lpf_index + excess;       /* 1‑based */
            seq_t p = (seq_t)g_primes[final_idx - 1];
            *res *= p;
        }
        return;
    }

    uint64_t mult = 1;
    for (uint32_t e = 0; e <= exp[idx]; ++e) {
        gen_divs(idx + 1, prod * mult, res, pf, exp, tot);
        mult *= pf[idx];
    }
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;            /* a(1) = 1 */

    ensure_precompute(n);

    /* factorise n using precomputed smallest prime factor table */
    idx_t tmp = n;
    uint32_t pf[20];   /* enough for n up to ~10^6 */
    uint32_t exp[20];
    int tot = 0;

    while (tmp > 1) {
        uint32_t p = g_spf[(idx_t)tmp];
        uint32_t c = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++c;
        }
        pf[tot]   = p;
        exp[tot]  = c;
        ++tot;
    }

    seq_t result = (seq_t)1;
    gen_divs(0, (uint64_t)1, &result, pf, exp, tot);
    return result;
}

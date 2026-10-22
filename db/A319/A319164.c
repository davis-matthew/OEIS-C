/*
 * A319164  Number of integer partitions of n that are neither relatively
 *          prime nor aperiodic.
 * https://oeis.org/A319164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A319164
   Number of integer partitions of n that are neither relatively prime nor aperiodic.
   Formula: a(n) = Σ_{d|n , d>1} ( -2 μ(d) + (μ * μ)(d) ) · p(n/d)
   where μ is the Möbius function, (μ * μ) is its Dirichlet convolution,
   and p(k) is the ordinary partition number.  */
#include <stdint.h>
#include <stdlib.h>

/* Pre‑computed static arrays */
static seq_t *p_arr   = NULL;   /* partition numbers */
static int8_t *mu_arr = NULL;   /* Möbius function μ(d)   */
static int8_t *g_arr  = NULL;   /* (μ * μ)(d) values     */
static size_t  max_comp = 0;    /* largest index already computed */

/* Ensure all tables are computed up to at least n */
static void ensure_precompute(idx_t n)
{
    if ((idx_t)max_comp >= n) return;

    /* Allocate new tables */
    size_t nsz = (size_t)(n + 1);
    seq_t  *new_p  = (seq_t *)calloc(nsz, sizeof(seq_t));
    int8_t *new_mu = (int8_t *)calloc(nsz, sizeof(int8_t));
    int8_t *new_g  = (int8_t *)calloc(nsz, sizeof(int8_t));
    if (!new_p || !new_mu || !new_g) abort();

    /* ----- compute Möbius μ via linear sieve ----- */
    uint32_t *spfs = (uint32_t *)calloc(nsz, sizeof(uint32_t));
    uint32_t *primes = (uint32_t *)malloc(nsz * sizeof(uint32_t));
    size_t primes_cnt = 0;
    new_mu[1] = 1;
    for (size_t i = 2; i <= (size_t)n; ++i) {
        if (spfs[i] == 0) {            /* i is prime */
            spfs[i] = (uint32_t)i;
            new_mu[i] = -1;
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (prod > (uint64_t)n) break;
            spfs[prod] = p;
            if (i % p == 0) {
                new_mu[prod] = 0;
                break;
            } else {
                new_mu[prod] = (int8_t)(-new_mu[i]);
            }
        }
    }

    /* ----- compute (μ * μ)(d) via factorisation ----- */
    new_g[1] = 1;
    for (size_t i = 2; i <= (size_t)n; ++i) {
        size_t tmp = i;
        int8_t val = 1;
        for (; tmp > 1; ) {
            uint32_t prime = spfs[tmp];
            int exp = 0;
            while (tmp % prime == 0) {
                tmp /= prime;
                ++exp;
            }
            if (exp >= 3) {            /* any exponent ≥3 makes (μ*μ)=0 */
                val = 0;
                break;
            }
            /* contribution of a prime power */
            if (exp == 1) val *= -2;   /* μ(p) = –1 ⇒ (μ*μ)(p) = –2 */
            else          val *= 1;    /* μ(p²)=0   ⇒ (μ*μ)(p²)=1  */
        }
        new_g[i] = val;
    }

    /* ----- compute partition numbers p(k) up to n ----- */
    new_p[0] = 1;
    for (size_t m = 1; m <= (size_t)n; ++m) {
        seq_t acc = 0;
        for (int k = 1; ; ++k) {
            size_t pent1 = (size_t)k * (3 * (size_t)k - 1) / 2;
            if (pent1 > m) break;
            seq_t term = new_p[m - pent1];
            if (k & 1) acc += term; else acc -= term;
            size_t pent2 = (size_t)k * (3 * (size_t)k + 1) / 2;
            if (pent2 > m) continue;
            term = new_p[m - pent2];
            if (k & 1) acc += term; else acc -= term;
        }
        new_p[m] = acc;
    }

    /* Free old tables */
    free(p_arr);
    free(mu_arr);
    free(g_arr);
    free(primes);
    free(spfs);

    /* Store new tables */
    p_arr = new_p;
    mu_arr = new_mu;
    g_arr = new_g;
    max_comp = n;
}

/* Return a(n) */
seq_t a(idx_t n)
{
    ensure_precompute(n);

    seq_t pos = 0;
    seq_t neg = 0;

    /* enumerate all divisors > 1 of n */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;

        /* first divisor i */
        idx_t d1 = i;
        if (d1 > 1) {
            int coeff = -2 * (int)mu_arr[(size_t)d1] + (int)g_arr[(size_t)d1];
            seq_t term = p_arr[(size_t)(n / d1)] * (seq_t)((coeff >= 0) ? coeff : -coeff);
            if (coeff >= 0) pos += term; else neg += term;
        }

        /* paired divisor n/i, if distinct */
        idx_t d2 = n / i;
        if (d2 != d1 && d2 > 1) {
            int coeff = -2 * (int)mu_arr[(size_t)d2] + (int)g_arr[(size_t)d2];
            seq_t term = p_arr[(size_t)(n / d2)] * (seq_t)((coeff >= 0) ? coeff : -coeff);
            if (coeff >= 0) pos += term; else neg += term;
        }
    }

    /* final result: pos - neg  (guaranteed non‑negative) */
    if (pos < neg) return 0;   /* safety, should not occur */
    return pos - neg;
}

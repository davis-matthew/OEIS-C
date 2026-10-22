/*
 * A339672  Number of partitions of n into 7 distinct and relatively prime
 *          parts.
 * https://oeis.org/A339672
 *
 * Offset:    28    (a(28) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A339672 – Number of partitions of n into 7 distinct, pairwise
   relatively prime parts.
   Formula (OEIS): a(n) = Σ_{d|n} μ(d) * p7(n/d − 28)
   where p7(t) is the number of partitions of t into parts 1..7.
   p7(t) is computed by the usual coin‑change DP.  μ(d) is Möbius function. */
/* ----------------------------------------------------------------------- */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ----------  partitions into parts 1..7  -------------------------------- */
static seq_t *p7 = NULL;          /* p7[t]  for 0 <= t <= current max */
static size_t p7_len = 0;          /* current size of p7 array */

/* Ensure p7[t] available up to max_t (inclusive).  */
static void ensure_p7_up_to(idx_t max_t)
{
    size_t need = (size_t)max_t + 1;
    if (need <= p7_len) return;          /* already computed */

    /* Allocate & compute fresh DP of required size.  */
    seq_t *newp = (seq_t *)malloc(need * sizeof(seq_t));
    if (!newp) abort();

    memset(newp, 0, need * sizeof(seq_t));
    newp[0] = 1;                         /* one way to make 0 */

    for (idx_t coin = 1; coin <= 7; ++coin) {
        for (idx_t i = coin; i <= max_t; ++i) {
            newp[i] += newp[i - coin];
        }
    }

    free(p7);
    p7 = newp;
    p7_len = need;
}

/* ----------  Möbius function  ------------------------------------------ */
static int8_t *mu = NULL;           /* μ[0..current max]            */
static size_t mu_len = 0;           /* current size of mu array     */
static uint64_t *spf = NULL;        /* smallest prime factor array   */
static size_t spf_len = 0;

/* Ensure spf array up to limit.  */
static void ensure_spf_up_to(idx_t limit)
{
    if (limit < spf_len) return;

    free(spf);
    spf_len = limit + 1;
    spf = (uint64_t *)calloc(spf_len, sizeof(uint64_t));
    if (!spf) abort();

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == 0) {           /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == 0) spf[i] = i;
}

/* Ensure Möbius function values up to limit.  */
static void ensure_mu_up_to(idx_t limit)
{
    if (limit < mu_len) return;

    free(mu);
    mu_len = limit + 1;
    mu = (int8_t *)malloc(mu_len * sizeof(int8_t));
    if (!mu) abort();

    ensure_spf_up_to(limit);

    mu[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        uint64_t x = i;
        int8_t val = 1;
        while (x > 1) {
            uint64_t p = spf[x];
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                cnt++;
                if (cnt > 1) {            /* squared prime factor */
                    val = 0;
                    break;
                }
            }
            if (val == 0) break;
            val = -val;                   /* flip sign for each distinct prime */
        }
        mu[i] = val;
    }
}

/* ----------  main sequence function  ----------------------------------- */
seq_t a(idx_t n)
{
    if (n < 28) return 0;                 /* offset requirement */

    /* Prepare needed tables. */
    ensure_p7_up_to((size_t)n - 28);
    ensure_mu_up_to(n);

    seq_t sum_plus  = 0;
    seq_t sum_minus = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;

        /* first divisor d1 = i */
        idx_t d1 = i;
        if (d1 <= n / 28) {
            int8_t mu_val = mu[d1];
            if (mu_val != 0) {
                idx_t t = n / d1 - 28;
                if (mu_val > 0)
                    sum_plus += p7[t];
                else
                    sum_minus += p7[t];
            }
        }

        /* counterpart divisor d2 = n / i */
        idx_t d2 = n / i;
        if (d2 == d1) continue;
        if (d2 <= n / 28) {
            int8_t mu_val = mu[d2];
            if (mu_val != 0) {
                idx_t t = n / d2 - 28;
                if (mu_val > 0)
                    sum_plus += p7[t];
                else
                    sum_minus += p7[t];
            }
        }
    }

    return sum_plus - sum_minus;
}

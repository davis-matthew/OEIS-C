/*
 * A373924  a(n) = Sum_{d|n} (-1)^pi(d) * mu(n/d).
 * https://oeis.org/A373924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^pi(d) * mu(n/d)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation precomputes all required data up to the
 * current maximum argument value using a linear sieve.  It stores the
 * smallest prime factor (spf), the Mobius function (mu) and the prime
 * counting function (pi) for indices up to the maximum evaluated n.
 * Divisors of n are generated via its prime factorisation; for each
 * divisor d we lookup pi[d] and mu[n/d] directly from the precomputed
 * tables and accumulate the sum.  All arithmetic uses the seq_t type
 * provided by the build.  The code is valid C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ----  Static data used for the sieve  ---- */
static idx_t *spf          = NULL;   /* smallest prime factor of i  */
static int8_t *mu_arr      = NULL;   /* Möbius function value of i   */
static idx_t *pi_arr       = NULL;   /* number of primes ≤ i         */
static uint32_t *primes    = NULL;   /* list of primes, size = max+1 */
static size_t   prime_cnt  = 0;
static idx_t   max_limit   = 0;

/* ----  Ensure the sieve tables are available up to `limit`  ---- */
static void ensure_sieve(idx_t limit)
{
    if (limit <= max_limit) return;

    /* Allocate new tables.  Size is limit+1 for 1‑based indexing.        */
    idx_t  *new_spf   = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    int8_t *new_mu    = (int8_t *)malloc((size_t)(limit + 1) * sizeof(int8_t));
    idx_t  *new_pi    = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    uint32_t *new_pr  = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));

    if (!new_spf || !new_mu || !new_pi || !new_pr) return;  /* unlikely */

    memset(new_spf, 0, (size_t)(limit + 1) * sizeof(idx_t));
    new_mu[1] = 1;          /* μ(1) = 1  */
    new_spf[1] = 1;
    new_pi[0] = 0;          /* π(0) = 0  */
    new_pi[1] = 0;          /* π(1) = 0  */
    new_pr[0] = 0;          /* unused  */
    prime_cnt = 0;

    /* Linear sieve – compute spf, mu and the prime list. */
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {               /* i is prime */
            new_spf[i] = i;
            new_mu[i]  = -1;
            new_pr[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = new_pr[j];
            idx_t ip = i * (idx_t)p;
            if (ip > limit) break;
            new_spf[ip] = p;
            if (i % p == 0) {
                new_mu[ip] = 0;
                break;
            } else {
                new_mu[ip] = -new_mu[i];
            }
        }
    }

    /* Compute π(i) as a prefix sum of the primality indicator. */
    for (idx_t i = 2; i <= limit; ++i) {
        new_pi[i] = new_pi[i - 1] + (new_spf[i] == i ? 1 : 0);
    }

    /* Replace old tables with the new ones.  Old tables are freed later. */
    free(spf);
    free(mu_arr);
    free(pi_arr);
    free(primes);

    spf = new_spf;
    mu_arr = new_mu;
    pi_arr = new_pi;
    primes = new_pr;
    max_limit = limit;
}

/* ----  Recursive generation of all divisors of n  ---- */
static void rec_divs(const uint32_t *pf, const int *exps, size_t cnt,
                     size_t idx, idx_t curd, idx_t n,
                     seq_t *sum)
{
    if (idx == cnt) {
        idx_t q = n / curd;                     /* n / d */
        int   pi_parity = (int)(pi_arr[curd] & 1);
        int   sign_pi   = pi_parity ? -1 : 1;
        int8_t mu_q      = mu_arr[q];
        *sum += (seq_t)(sign_pi * mu_q);
        return;
    }

    uint32_t p = pf[idx];
    int exp = exps[idx];
    idx_t d = curd;
    for (int e = 0; e <= exp; ++e) {
        rec_divs(pf, exps, cnt, idx + 1, d, n, sum);
        if (e == exp) break;
        d *= (idx_t)p;  /* multiply by another p */
    }
}

/* ----  Public function implementing the OEIS sequence  ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* unused, but harmless */

    /* Make sure all auxiliary tables are ready up to n. */
    ensure_sieve(n);

    /* Factorise `n` using the precomputed spf table. */
    uint32_t pf[64];
    int     exps[64];
    size_t  cnt = 0;
    idx_t   tmp = n;
    while (tmp > 1) {
        uint32_t p = (uint32_t)spf[tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        pf[cnt]   = p;
        exps[cnt] = e;
        ++cnt;
    }

    /* Enumerate all divisors and accumulate the required sum. */
    seq_t result = 0;
    rec_divs(pf, exps, cnt, 0, (idx_t)1, n, &result);
    return result;
}

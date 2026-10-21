/*
 * A242457  Least number k such that k!/n is prime, or 0 if no such number
 *          exists.
 * https://oeis.org/A242457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that k!/n is prime, 0 if none
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _legendre
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_legendre.h"

/* ------------------------------------------------------------------ */
/* Small sieve of primes up to 20000 (enough for n up to 10000).      */
/* ------------------------------------------------------------------ */
#define PRIME_MAX 20000
static bool _prime_sieved = false;
static idx_t _prime_list[PRIME_MAX];
static size_t _prime_cnt;

/* generate primes at first use */
static void _ensure_primes(void)
{
    if (_prime_sieved) return;
    _prime_sieved = true;
    bool is_composite[PRIME_MAX] = {0};
    _prime_cnt = 0;
    for (idx_t i = 2; i < PRIME_MAX; ++i) {
        if (!is_composite[i]) {
            _prime_list[_prime_cnt++] = i;
            if (i * i < PRIME_MAX) {
                for (idx_t j = i * i; j < PRIME_MAX; j += i)
                    is_composite[j] = true;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Factorize n into primes <= sqrt(n).                                 */
/* ------------------------------------------------------------------ */
static void _factor_uint64(idx_t n, idx_t *pf, idx_t *pw, size_t *cnt)
{
    *cnt = 0;
    if (n <= 1) return;
    for (size_t i = 0; i < _prime_cnt && _prime_list[i] * _prime_list[i] <= n; ++i) {
        idx_t p = _prime_list[i];
        if (n % p == 0) {
            idx_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pf[*cnt] = p;
            pw[*cnt] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        pf[*cnt] = n;
        pw[*cnt] = 1;
        ++(*cnt);
    }
}

/* ------------------------------------------------------------------ */
/* Legendre exponent of prime p in k!                                  */

/* ------------------------------------------------------------------ */
/* Main OEIS A242457 function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n must be >=1 per specification                        */
    if (n == 0) return 0;                /* safety, unused */

    /* Ensure prime list is available                           */
    _ensure_primes();

    /* ------------------------------------------------------------------
       Factor n and pre‑compute its prime powers.                     */
    /* ------------------------------------------------------------------ */
    idx_t n_pr[64], n_pw[64];
    size_t n_pf_cnt;
    _factor_uint64(n, n_pr, n_pw, &n_pf_cnt);

    /* Get exponent of each prime of n in n itself */
    /* (at most 64 distinct primes for n <= 10^4) */

    /* ------------------------------------------------------------------
       Compute minimal k such that k! contains all prime powers of n.
       This is the lower bound for our search.                         */
    /* ------------------------------------------------------------------ */
    idx_t k_min = 1;
    for (size_t i = 0; i < n_pf_cnt; ++i) {
        idx_t p = n_pr[i];
        idx_t need = n_pw[i];
        idx_t k = p;
        while (_legendre(k, p) < need) ++k;
        if (k > k_min) k_min = k;
    }

    /* Upper bound: n+2 per the OEIS note */
    idx_t k_max = n + 2;
    if (k_min > k_max) return 0;

    /* ------------------------------------------------------------------
       Search for the least k in [k_min, k_max] whose quotient k!/n is
       a prime.  For each k, we compare the exponent vector of k! with
       that of n.  The quotient is prime iff exactly one prime p has
       exponent e_n(p)+1 in k! and all other primes match e_n(q).
       ------------------------------------------------------------------ */
    for (idx_t k = k_min; k <= k_max; ++k) {
        size_t diff_cnt = 0;      /* how many primes differ */
        idx_t diff_prime = 0;     /* which prime differs */
        bool bad = false;

        /* Iterate over all primes <= k */
        for (size_t i = 0; i < _prime_cnt && _prime_list[i] <= k; ++i) {
            idx_t p = _prime_list[i];
            idx_t e_k = _legendre(k, p);

            /* Find exponent of p in n, if any */
            idx_t e_n = 0;
            for (size_t j = 0; j < n_pf_cnt; ++j)
                if (n_pr[j] == p) { e_n = n_pw[j]; break; }

            idx_t d = e_k - e_n;

            if (d == 0) continue;
            else if (d == 1) {
                ++diff_cnt;
                if (diff_cnt == 1) diff_prime = p;
                else { bad = true; break; }
            }
            else { bad = true; break; }
        }

        if (!bad && diff_cnt == 1) {
            return (seq_t)k;   /* found smallest valid k */
        }
    }

    return 0;   /* no such k in the allowed range */
}

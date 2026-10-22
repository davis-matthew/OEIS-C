/*
 * A389214  Numbers k such that A048146(k)+A162296(k) >= 2*k, where A048146 is
 *          the sum of non-unitary divisors of n, and A162296 is the sum of
 *          divisors of n that have a square factor.
 * https://oeis.org/A389214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer k such that * 2*σ(k) ≥ 2*k + U(k) + ∏(1 +
 *            p_i) * where σ is the sum of all divisors, * U is the sum of
 *            unitary divisors, and the product runs over all primes p_i
 *            dividing k
 * Range:     u8 n <= 8, u16 n <= 1202, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_capacity_u64.h"

/* ---------- SPF (smallest prime factor) sieve ---------- */
static uint64_t *spf        = NULL;
static size_t     spf_cap    = 0;
static uint64_t *prime_list = NULL;
static size_t     prime_cap  = 0;
static size_t     prime_cnt  = 0;
static uint64_t   spf_limit  = 0;

static void ensure_spf(uint64_t limit)
{
    if (limit <= spf_limit) return;

    /* expand spf array if needed */
    if ((uint64_t)(spf_cap) < limit + 1) {
        spf_cap = limit + 1;
        spf = (uint64_t*)realloc(spf, spf_cap * sizeof(uint64_t));
    }
    /* initialise newly added part to 0 */
    for (uint64_t i = spf_limit + 1; i <= limit; ++i)
        spf[i] = 0;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                    /* i is prime */
            spf[i] = i;
            if (prime_cnt >= prime_cap) {
                prime_cap = prime_cap ? prime_cap << 1 : 128;
                prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t p = prime_list[j];
            if (p > spf[i] || i * p > limit) break;
            spf[i * p] = p;
        }
    }
    spf_limit = limit;
}

/* ---------- sequence storage ---------- */
static uint64_t *term_arr = NULL;
static size_t   term_len = 0, term_cap = 0;
static uint64_t  max_checked = 0;

/* ---------- the required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* safeguard, offset is 1 */

    while (term_len < (size_t)n) {
        ++max_checked;
        ensure_spf(max_checked);

        seq_t sigma      = 1;
        seq_t unitary_sum = 1;
        seq_t sum_sf     = 1;           /* sum of square‑free divisors */

        uint64_t m = max_checked;
        while (m > 1) {
            uint64_t p = spf[m];
            int exp = 0;
            while (m % p == 0) { m /= p; ++exp; }

            seq_t p_pow = 1;
            for (int i = 0; i < exp; ++i) p_pow *= (seq_t)p;          /* p^e */

            /* σ contribution: (p^(e+1)-1)/(p-1) */
            seq_t numerator   = p_pow * (seq_t)p - 1;
            seq_t denominator = (seq_t)p - 1;
            seq_t sigma_term  = numerator / denominator;
            sigma *= sigma_term;

            /* unitary sum contribution: 1 + p^e */
            unitary_sum *= ((seq_t)1 + p_pow);

            /* square‑free sum contribution: 1 + p */
            sum_sf *= ((seq_t)1 + (seq_t)p);
        }

        seq_t left  = sigma * (seq_t)2;
        seq_t right = (seq_t)max_checked * (seq_t)2 + unitary_sum + sum_sf;

        if (left >= right) {
            ensure_capacity_u64(&term_arr, &term_cap, term_len + 1);
            term_arr[term_len++] = max_checked;
        }
    }
    return (seq_t)term_arr[n - 1];
}

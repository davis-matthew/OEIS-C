/*
 * A383177  Sphenic numbers k such that floor(log(k)/log(lpf(k))) =
 *          1+floor(log(k)/log(p)) for all primes p | k such that p > lpf(k),
 *          where lpf = A020639(k).
 * https://oeis.org/A383177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that *
 *            ──────────────────────────────────────────────────────────────────────
 *            * floor(log(k)/log(lpf(k))) = 1 + floor(log(k)/log(p)) * for all
 *            primes p | k with p > lpf(k) * where lpf(k) is the least prime
 *            factor of k. * * This is equivalent to the inequalities * p0...
 * Range:     u8 none, u16 n <= 183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/append_candidate.h"

/* static state */
static seq_t *seq_values = NULL; /* cached sequence values */
static size_t seq_len   = 0;
static size_t seq_cap   = 0;

static uint32_t *spf = NULL;          /* smallest prime factor array */
static const size_t SPF_LIMIT = 20000000;   /* 20 M (≈80 MB of memory) */
static bool spf_ready = false;

static idx_t last_checked = 2;   /* next integer to test */

/*--- build a linear sieve for SPF ----------------------------------------*/
static void init_spf(void)
{
    if (spf_ready) return;
    size_t n = SPF_LIMIT;
    spf = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
    if (!spf) { abort(); }
    for (size_t i = 0; i <= n; ++i) spf[i] = 0;

    size_t primes_cap = 100000;
    uint32_t *primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    size_t primes_cnt = 0;

    for (size_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!primes) abort();
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            if (p > spf[i] || (uint64_t)i * p > n) break;
            spf[i * p] = p;
        }
    }
    free(primes);
    spf_ready = true;
}

/*--- generate terms up to required index -------------------------------*/
static void ensure_terms(idx_t n)
{
    if (seq_len >= (size_t)n) return;
    init_spf();

    while (seq_len < (size_t)n) {
        uint64_t cand = last_checked++;
        if (cand > SPF_LIMIT) { /* should never happen with the chosen limit */
            abort();
        }

        uint64_t m = cand;
        uint64_t prime[3] = {0, 0, 0};
        int cnt = 0;
        while (m > 1 && cnt < 3) {
            uint32_t p = spf[(size_t)m];
            prime[cnt++] = p;
            while (m % p == 0) m /= p;
        }
        if (cnt == 3 && m == 1) {          /* product of three distinct primes */
            /* sort the primes ascending: a ≤ b ≤ c */
            uint64_t a = prime[0], b = prime[1], c = prime[2];
            if (a > b) { uint64_t t = a; a = b; b = t; }
            if (b > c) { uint64_t t = b; b = c; c = t; }
            if (a > b) { uint64_t t = a; a = b; b = t; }

            unsigned __int128 val = cand;
            unsigned __int128 a2 = (unsigned __int128)a * a;
            unsigned __int128 a3 = a2 * a;
            unsigned __int128 a4 = a3 * a;
            if (val < a3 || val >= a4) continue;   /* a³ ≤ k < a⁴ */

            unsigned __int128 b2 = (unsigned __int128)b * b;
            unsigned __int128 b3 = b2 * b;
            if (val < b2 || val >= b3) continue;   /* b² ≤ k < b³ */

            unsigned __int128 c2 = (unsigned __int128)c * c;
            unsigned __int128 c3 = c2 * c;
            if (val < c2 || val >= c3) continue;   /* c² ≤ k < c³ */

            /* k satisfies all conditions */
            append_candidate(&seq_values, &seq_len, &seq_cap, (seq_t)cand);
        }
    }
}

/*--- required function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0; return 0 */
    ensure_terms(n);
    return seq_values[n - 1];
}

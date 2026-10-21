/*
 * A281294  Refactorable numbers k such that 2*k + 1 is also a refactorable
 *          number.
 * https://oeis.org/A281294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 221, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A281294: Refactorable numbers k such that 2*k+1
   is also a refactorable number (hence 2*k+1 must be a perfect square). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----------  Prime generation utilities  ---------- */
static uint64_t *primes      = NULL;   /* list of primes  */
static size_t    primes_len  = 0;
static size_t    primes_cap  = 0;
static uint64_t  last_checked = 2;

static void init_primes(void)
{
    if (!primes) {
        primes_cap = 64;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes_len = 1;
        last_checked = 2;
    }
}

static void generate_next_prime(void)
{
    uint64_t cand = last_checked + 1;
    if (cand % 2 == 0) cand++;                 /* skip even numbers */

    while (1) {
        bool prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) {
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = cand;
            last_checked = cand;
            return;
        }
        cand += 2;           /* next odd candidate */
    }
}

static void ensure_primes_up_to(uint64_t limit)
{
    /* generate primes until the last prime^2 > limit */
    while (primes_len == 0 || primes[primes_len - 1] * primes[primes_len - 1] <= limit) {
        generate_next_prime();
    }
}

/* ----------  Factorization utilities  ---------- */
#define MAX_FACTORS 20

static size_t factor_uint64(uint64_t n,
                            uint64_t primes_out[MAX_FACTORS],
                            int       exps_out[MAX_FACTORS])
{
    size_t cnt = 0;
    uint64_t temp = n;
    size_t idx = 0;

    while (1) {
        if (primes_len == 0) init_primes();

        if (idx >= primes_len) generate_next_prime();

        uint64_t p = primes[idx];
        if (p * p > temp) break;

        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            primes_out[cnt] = p;
            exps_out[cnt]   = e;
            ++cnt;
        }
        ++idx;
    }

    if (temp > 1) {
        primes_out[cnt] = temp;
        exps_out[cnt]   = 1;
        ++cnt;
    }
    return cnt;
}

/* ----------  Cache for sequence values  ---------- */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t cap = cache_cap ? cache_cap * 2 : 64;
    if (cap < needed) cap = needed;
    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    cache_cap = cap;
}

/* ----------  Main sequence function  ---------- */
static bool initialized = false;
static uint64_t current_s = 3;    /* current odd s to test */

static void init_sequence_state(void)
{
    init_primes();
    current_s = 3;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; behaviour below irrelevant */

    if (!initialized) init_sequence_state();

    while (cache_len < (size_t)n) {
        uint64_t s = current_s;
        current_s += 2;   /* next odd number */

        /* Factor s to compute divisor count of s^2 */
        uint64_t ps[MAX_FACTORS];
        int exp_s[MAX_FACTORS];
        size_t cnt_s = factor_uint64(s, ps, exp_s);

        /* d_square = product (2*exp + 1) */
        uint64_t d_square = 1;
        for (size_t i = 0; i < cnt_s; ++i) {
            d_square *= (2ULL * exp_s[i] + 1ULL);
        }

        /* Check if s^2 is divisible by d_square */
        unsigned __int128 sq128 = (unsigned __int128)s * (unsigned __int128)s;
        if (((uint64_t)sq128) % d_square != 0) continue;   /* square not refactorable */

        /* Compute a = (s-1)/2, b = s+1 */
        uint64_t a_val = (s - 1) / 2;
        uint64_t b_val = s + 1;

        /* Factor a and b */
        uint64_t pa[MAX_FACTORS], pb[MAX_FACTORS];
        int exp_a[MAX_FACTORS], exp_b[MAX_FACTORS];
        size_t cnt_a = factor_uint64(a_val, pa, exp_a);
        size_t cnt_b = factor_uint64(b_val, pb, exp_b);

        /* Merge exponents to compute d(k) */
        uint64_t primes_merged[MAX_FACTORS];
        int   exp_merged[MAX_FACTORS];
        size_t cnt_merged = 0;

        /* Copy a's factors */
        for (size_t i = 0; i < cnt_a; ++i) {
            primes_merged[cnt_merged] = pa[i];
            exp_merged[cnt_merged]   = exp_a[i];
            ++cnt_merged;
        }
        /* Merge b's factors */
        for (size_t i = 0; i < cnt_b; ++i) {
            bool found = false;
            for (size_t j = 0; j < cnt_merged; ++j) {
                if (primes_merged[j] == pb[i]) {
                    exp_merged[j] += exp_b[i];
                    found = true;
                    break;
                }
            }
            if (!found) {
                primes_merged[cnt_merged] = pb[i];
                exp_merged[cnt_merged]   = exp_b[i];
                ++cnt_merged;
            }
        }

        /* Compute d_k */
        unsigned __int128 d_k128 = 1;
        for (size_t i = 0; i < cnt_merged; ++i) {
            d_k128 *= (unsigned __int128)(exp_merged[i] + 1);
        }
        uint64_t d_k = (uint64_t)d_k128;   /* fits in 64-bit for our range */

        /* Check k % d_k == 0, where k = a_val * b_val */
        unsigned __int128 k128 = (unsigned __int128)a_val * (unsigned __int128)b_val;
        if ((uint64_t)k128 % d_k != 0) continue;   /* k not refactorable */

        /* Accept the term */
        uint64_t k_val = (uint64_t)k128;
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)k_val;
    }

    return cache[(size_t)n - 1];
}

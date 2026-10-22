/*
 * A371733  Maximal length of a factorization of n into factors > 1 all having
 *          the same sum of prime indices.
 * https://oeis.org/A371733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9328, u16 n <= 9328, u32 n <= 9328, u64 n <= 9328,
 *            u128 n <= 9328, big any n
 * Helpers:   cmp_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:  For each n, factorise it, build a multiset of the
 * prime‑indices of its prime factors (each occurrence counted).  Let
 * T be the sum of these indices.  For every divisor S of T we test
 * whether the multiset can be partitioned into groups whose sums are
 * all S.  If it can, the factorisation has k = T / S factors.
 * The maximal k (and thus a(n)) is the smallest S for which the
 * partition is possible.  The algorithm is exhaustive only on the
 * small multiset (length ≤ log₂ n, which is ≤13 for n ≤ 10000).  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_idx.h"

static uint64_t *prime_list = NULL;   /* array of primes          */
static size_t   prime_cnt  = 0;      /* number of primes         */
static uint64_t prime_max  = 0;      /* highest prime generated  */

static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_max) return;

    if (prime_list) free(prime_list);

    size_t n = (size_t)limit + 1;
    bool *is_prime = (bool*)malloc(n);
    if (!is_prime) { prime_list = NULL; prime_cnt = 0; prime_max = 0; return; }
    for (size_t i = 0; i < n; ++i) is_prime[i] = true;
    if (n > 0) is_prime[0] = false;
    if (n > 1) is_prime[1] = false;
    for (uint64_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;
    uint64_t *new_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    size_t pos = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_list[pos++] = i;
    free(is_prime);

    prime_list = new_list;
    prime_cnt  = cnt;
    prime_max  = limit;
}

static int prime_index_of(uint64_t p)
{
    if (p < 2) return -1;
    ensure_primes_upto(p);
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return (int)(mid + 1); /* indices start at 1 */
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return -1; /* p not prime – should not happen */
}

static uint64_t int_sqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) { ans = mid; lo = mid + 1; }
        else hi = mid - 1;
    }
    return ans;
}

/* Factorise n into primes[0..np-1] with exponents exp[0..np-1].  */
static void factor_uint64(uint64_t n, uint64_t *pr, int *ex, size_t *cnt)
{
    *cnt = 0;
    uint64_t m = n;
    uint64_t limit = int_sqrt_u64(m);
    ensure_primes_upto(limit);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
        uint64_t p = prime_list[i];
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            pr[*cnt] = p;
            ex[*cnt] = e;
            ++*cnt;
        }
    }
    if (m > 1) {          /* remaining prime factor */
        pr[*cnt] = m;
        ex[*cnt] = 1;
        ++*cnt;
    }
}

/* Comparator for qsort on idx_t values. */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    /* ---- 1. Factorise n and collect its prime‑indices ---- */
    uint64_t primes[64];
    int       exps[64];
    size_t    np = 0;
    factor_uint64((uint64_t)n, primes, exps, &np);

    int total_exp = 0;
    for (size_t i = 0; i < np; ++i) total_exp += exps[i];

    int *weights = (int*)malloc(total_exp * sizeof(int));
    int pos = 0;
    seq_t T = 0; /* total sum of indices */
    for (size_t i = 0; i < np; ++i) {
        int idx = prime_index_of(primes[i]);
        for (int e = 0; e < exps[i]; ++e) {
            weights[pos++] = idx;
            T += (seq_t)idx;
        }
    }

    /* ---- 2. Enumerate divisors of T ---- */
    idx_t *divs = NULL;
    size_t dlen = 0, dcap = 8;
    divs = (idx_t*)malloc(dcap * sizeof(idx_t));
    uint64_t limit_d = (uint64_t)int_sqrt_u64((uint64_t)T);
    for (uint64_t d = 1; d <= limit_d; ++d) {
        if ((uint64_t)T % d == 0) {
            if (dlen >= dcap) {
                dcap <<= 1;
                divs = (idx_t*)realloc(divs, dcap * sizeof(idx_t));
            }
            divs[dlen++] = d;
            uint64_t e = (uint64_t)T / d;
            if (e != d) {
                if (dlen >= dcap) {
                    dcap <<= 1;
                    divs = (idx_t*)realloc(divs, dcap * sizeof(idx_t));
                }
                divs[dlen++] = e;
            }
        }
    }
    qsort(divs, dlen, sizeof(idx_t), cmp_idx);

    /* ---- 3. Pre‑compute subset sums ---- */
    size_t max_mask = (1ULL << total_exp);
    int *sum_mask = (int*)calloc(max_mask, sizeof(int));
    for (size_t mask = 1; mask < max_mask; ++mask) {
        int s = 0;
        for (size_t bit = 0; bit < total_exp; ++bit)
            if (mask & (1U << bit))
                s += weights[bit];
        sum_mask[mask] = s;
    }

    /* ---- 4. Try divisors S in ascending order ---- */
    seq_t result = 1; /* at least the trivial factorisation itself */
    for (size_t i = 0; i < dlen; ++i) {
        idx_t S = divs[i];
        if ((seq_t)S == 0) continue;
        if ((seq_t)T % (seq_t)S != 0) continue;
        size_t k = (size_t)((seq_t)T / (seq_t)S);

        unsigned char *dp = (unsigned char*)calloc(max_mask, sizeof(unsigned char));
        dp[0] = 1;
        for (size_t mask = 1; mask < max_mask; ++mask) {
            if (sum_mask[mask] % (int)S != 0) continue;
            for (size_t sub = mask; sub; sub = (sub - 1) & mask) {
                if (sum_mask[sub] == (int)S && dp[mask ^ sub]) {
                    dp[mask] = 1;
                    break;
                }
            }
        }
        if (dp[max_mask - 1]) {
            result = (seq_t)k;     /* smallest S gives largest k */
            free(dp);
            break;
        }
        free(dp);
    }

    /* ---- 5. Clean up ---- */
    free(weights);
    free(divs);
    free(sum_mask);
    return result;
}

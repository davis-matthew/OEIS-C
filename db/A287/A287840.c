/*
 * A287840  Numbers that generate Carmichael numbers using Erdős's method.
 * https://oeis.org/A287840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A287840: Numbers generating Carmichael numbers via Erdős's method
   (search approach) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- sieve helpers ---------- */
static uint64_t *spf = NULL;        /* smallest prime factor array */
static size_t    sieve_limit = 0;   /* current sieve size */

/* enlarge sieve to cover numbers up to 'limit' (inclusive) */
static void extend_sieve(size_t limit)
{
    if (sieve_limit >= limit) return;

    size_t new_limit = (sieve_limit ? sieve_limit : 2);
    while (new_limit < limit) new_limit <<= 1;

    uint64_t *new_spf = (uint64_t*)calloc(new_limit + 1, sizeof(uint64_t));
    for (size_t i = 2; i <= new_limit; ++i) {
        if (!new_spf[i]) {
            new_spf[i] = i;
            if (i <= new_limit / i) {
                for (size_t j = i * i; j <= new_limit; j += i)
                    if (!new_spf[j]) new_spf[j] = i;
            }
        }
    }
    free(spf);
    spf = new_spf;
    sieve_limit = new_limit;
}

/* factorise n with spf; store primes in pf[64], exponents in exps[64];
   return number of distinct prime factors. */
static size_t factor_by_spf(uint64_t n, uint64_t pf[64], uint64_t exps[64])
{
    size_t cnt = 0;
    while (n > 1) {
        uint64_t p = spf[n];
        uint64_t e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        pf[cnt] = p;
        exps[cnt] = e;
        ++cnt;
    }
    return cnt;
}

/* generate all divisors of n given its factorisation;
   divisors are stored in divs[4096]; return count. */
static void gen_divisors(size_t idx, uint64_t cur,
                         uint64_t *divs, size_t *count,
                         const uint64_t *pf, const uint64_t *exps,
                         size_t pf_cnt)
{
    if (idx == pf_cnt) {
        divs[(*count)++] = cur;
        return;
    }
    uint64_t p = pf[idx];
    uint64_t val = cur;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divisors(idx + 1, val, divs, count, pf, exps, pf_cnt);
        if (e < exps[idx]) val *= p;
    }
}

/* 64‑bit modular multiplication avoiding overflow */
static inline uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* recursive search for a subset of P with product ≡ 1 (mod n) and size ≥ 3 */
static bool dfs_subset(size_t pos, size_t chosen,
                       uint64_t prod, uint64_t n,
                       const uint64_t *P, size_t Pcnt)
{
    if (chosen >= 3 && prod == 1ULL) return true;
    if (pos == Pcnt) return false;
    if (chosen + (Pcnt - pos) < 3) return false;   /* not enough left */

    /* include P[pos] */
    uint64_t prod_inc = mod_mul_uint64(prod, P[pos], n);
    if (dfs_subset(pos + 1, chosen + 1, prod_inc, n, P, Pcnt)) return true;

    /* skip P[pos] */
    return dfs_subset(pos + 1, chosen, prod, n, P, Pcnt);
}

/* does n generate a Carmichael number by Erdős's method?  */
static bool qualifies(uint64_t n)
{
    /* ensure sieve covers at least n+1 */
    extend_sieve(n + 1);

    uint64_t pf[64], exps[64];
    size_t pf_cnt = factor_by_spf(n, pf, exps);

    uint64_t divs[4096];
    size_t divs_len = 0;
    gen_divisors(0, 1ULL, divs, &divs_len, pf, exps, pf_cnt);

    /* build set P of primes p with (p‑1)|n and p∤n */
    uint64_t P[64];
    size_t Pcnt = 0;
    for (size_t i = 0; i < divs_len; ++i) {
        uint64_t d = divs[i];
        uint64_t p = d + 1;
        if (p > n + 1) continue;   /* safety */
        if (spf[p] == p && (n % p) != 0) {
            if (Pcnt < 64) P[Pcnt++] = p;
        }
    }
    if (Pcnt < 3) return false;

    return dfs_subset(0, 0, 1ULL, n, P, Pcnt);
}

/* ---------- caching of the sequence ---------- */
static seq_t *term_cache = NULL;
static size_t term_len  = 0;
static size_t term_cap  = 0;
static uint64_t checked_until = 0;

/* ensure term_cache has capacity at least 'need' */
static void term_ensure_cap(size_t need)
{
    if (term_cap >= need) return;
    size_t new_cap = term_cap ? term_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *ptr = (seq_t*)realloc(term_cache, new_cap * sizeof(seq_t));
    term_cache = ptr;
    term_cap = new_cap;
}

/* --------- public API required by the judge --------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */

    /* first‑time initialisation */
    static bool init_once = false;
    if (!init_once) {
        extend_sieve(1000);        /* small start */
        term_cache = (seq_t*)malloc(16 * sizeof(seq_t));
        term_cap   = 16;
        term_len   = 0;
        checked_until = 0;
        init_once = true;
    }

    while (term_len < (size_t)n) {
        ++checked_until;
        if (qualifies(checked_until)) {
            term_ensure_cap(term_len + 1);
            term_cache[term_len++] = (seq_t)checked_until;
        }
    }
    return term_cache[n - 1];
}

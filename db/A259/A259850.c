/*
 * A259850  Numbers k such that k/phi(k) equals sigma(x)/x for some x<=k.
 * https://oeis.org/A259850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 528, u32 n <= 1230, u64 n <= 1230,
 *            u128 n <= 1230, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS A259850.
   It generates the sequence incrementally: for each k it
   stores the ratio σ(k)/k in a hash set, then checks if
   k/φ(k) matches any stored ratio (with x ≤ k).  The terms
   are cached so that repeated calls are fast.  The implementation
   uses only integer arithmetic and the utility functions provided. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static const size_t SPF_LIMIT = 2000000;   /* pre‑computed spf up to this limit */
static const size_t HASH_SIZE  = 1u << 20; /* 1,048,576 buckets */

typedef struct { seq_t num; seq_t den; } ratio_t;

static uint32_t *spf          = NULL;      /* smallest prime factor */
static size_t   spf_limit     = 0;
static uint32_t *primes       = NULL;      /* list of primes up to SPF_LIMIT */
static size_t   primes_cnt    = 0;

static ratio_t *hash_table    = NULL;
static size_t   hash_mask     = 0;

static seq_t   *terms         = NULL;      /* sequence values */
static size_t  terms_len      = 0;
static size_t  terms_cap      = 0;

static size_t  cur_k          = 0;         /* last processed k */

/* linear sieve to fill spf[0..limit] and primes list */
static void init_spf(void)
{
    if (spf != NULL)
        return;
    uint32_t *tmp_primes = (uint32_t *)malloc((SPF_LIMIT / 10 + 1) * sizeof(uint32_t));
    spf = (uint32_t *)calloc(SPF_LIMIT + 1, sizeof(uint32_t));
    spf[0] = 0;
    spf[1] = 1;
    size_t pi = 0;
    for (size_t i = 2; i <= SPF_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (pi == (size_t)(SPF_LIMIT / 10 + 1))
                tmp_primes = (uint32_t *)realloc(tmp_primes, (pi + SPF_LIMIT / 10 + 1) * sizeof(uint32_t));
            tmp_primes[pi++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pi && (long long)i * tmp_primes[j] <= (long long)SPF_LIMIT; ++j) {
            size_t v = i * tmp_primes[j];
            spf[v] = tmp_primes[j];
            if (i % tmp_primes[j] == 0)
                break;
        }
    }
    primes = tmp_primes;
    primes_cnt = pi;
    spf_limit = SPF_LIMIT;
}

/* simple hash for 64‑bit values; works for seq_t up to 64‑bit */
static size_t hash_key(seq_t a, seq_t b)
{
    /* cast to unsigned long long for hashing */
    uint64_t x = (uint64_t)a;
    uint64_t y = (uint64_t)b;
    return (size_t)((x * 1199540891u + y * 7269857u) & hash_mask);
}

/* insert a reduced ratio (num/den) in the hash table if not present */
static void hash_insert(seq_t num, seq_t den)
{
    size_t h = hash_key(num, den);
    for (;;) {
        if (hash_table[h].num == 0) { /* empty slot */
            hash_table[h].num = num;
            hash_table[h].den = den;
            return;
        }
        if (hash_table[h].num == num && hash_table[h].den == den)
            return;                 /* already present */
        h = (h + 1) & hash_mask;
    }
}

/* search for a reduced ratio (num/den) in the hash table */
static bool hash_contains(seq_t num, seq_t den)
{
    size_t h = hash_key(num, den);
    for (;;) {
        if (hash_table[h].num == 0)      /* empty slot -> not found */
            return false;
        if (hash_table[h].num == num && hash_table[h].den == den)
            return true;
        h = (h + 1) & hash_mask;
    }
}

/* add a new value to the dynamic sequence array */
static void add_term(seq_t val)
{
    if (terms_len == terms_cap) {
        terms_cap = (terms_cap == 0 ? 64 : terms_cap << 1);
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = val;
}

/* generate terms until at least target_n terms are obtained */
static void generate_to(size_t target_n)
{
    while (terms_len < target_n) {
        ++cur_k;
        /* factorise cur_k using spf */
        idx_t   x     = cur_k;
        seq_t   phi   = (seq_t)cur_k;
        seq_t   sigma = 1;

        while (x > 1) {
            uint32_t p = spf[x];
            size_t   e = 0;
            seq_t    p_pow = 1;
            seq_t    sigma_factor = 1;
            seq_t    term = 1;
            while (x % p == 0) {
                x /= p;
                ++e;
                p_pow *= (seq_t)p;
                term *= (seq_t)p;
                sigma_factor += term;
            }
            phi -= phi / (seq_t)p;
            sigma *= sigma_factor;
        }

        /* store σ(k)/k in the hash set */
        seq_t num_sigma = sigma;
        seq_t den_sigma = (seq_t)cur_k;
        seq_t g_sigma   = gcd(num_sigma, den_sigma);
        num_sigma /= g_sigma;
        den_sigma /= g_sigma;
        hash_insert(num_sigma, den_sigma);

        /* compute k/φ(k) ratio and test membership */
        seq_t num_k = (seq_t)cur_k;
        seq_t den_k = phi;
        seq_t g_k   = gcd(num_k, den_k);
        num_k /= g_k;
        den_k /= g_k;
        if (hash_contains(num_k, den_k))
            add_term((seq_t)cur_k);
    }
}

/* public entry point required by the framework */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* initialise structures on the first call */
    if (spf == NULL) {
        init_spf();
        hash_table = (ratio_t *)calloc(HASH_SIZE, sizeof(ratio_t));
        hash_mask = HASH_SIZE - 1;
    }

    if (n > terms_len)
        generate_to((size_t)n);

    return terms[n - 1];
}

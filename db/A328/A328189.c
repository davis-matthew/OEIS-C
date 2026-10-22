/*
 * A328189  Numbers n with at least one pair of consecutive divisible
 *          nontrivial divisors (greater than 1 and less than n).
 * https://oeis.org/A328189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328189: Numbers n with at least one pair of consecutive divisible nontrivial divisors */
/* The algorithm iteratively searches for valid numbers, keeping a cache of found terms so
   that subsequent calls reuse previous work.  Candidate numbers are factorised with a
   smallest‑prime‑factor (spf) sieve, all non‑trivial divisors are generated and sorted,
   and consecutive pairs are examined for divisibility. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ---------- helper: compare for qsort ---------- */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t v1 = *(const uint64_t *)a;
    uint64_t v2 = *(const uint64_t *)b;
    if (v1 < v2) return -1;
    if (v1 > v2) return  1;
    return 0;
}

/* ---------- global data for spf sieve and primes ---------- */
static uint64_t *g_spf = NULL;        /* smallest prime factor array */
static size_t g_spf_size = 0;         /* current size (elements 0..size-1) */
static uint64_t *g_primes = NULL;     /* list of primes found */
static size_t g_prime_count   = 0;
static size_t g_prime_cap     = 0;

/* extend the spf sieve to cover at least up to 'limit' (inclusive) */
static void extend_spf(size_t limit)
{
    if (limit < g_spf_size) return;          /* already covered */

    size_t new_size = limit + 1;

    uint64_t *new_spf = (uint64_t *)malloc(new_size * sizeof(uint64_t));
    if (!new_spf) abort();

    /* copy old data */
    if (g_spf)
        memcpy(new_spf, g_spf, g_spf_size * sizeof(uint64_t));

    /* initialize new region */
    for (size_t i = g_spf_size; i < new_size; ++i)
        new_spf[i] = 0;

    /* sieve for spf */
    for (size_t i = 2; i < new_size; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint64_t)i;
            /* insert prime i into list */
            if (g_prime_count == g_prime_cap) {
                g_prime_cap = g_prime_cap ? g_prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(g_primes, g_prime_cap * sizeof(uint64_t));
                if (!tmp) abort();
                g_primes = tmp;
            }
            g_primes[g_prime_count++] = (uint64_t)i;
        }
        uint64_t fi = new_spf[i];
        for (size_t pi = 0; pi < g_prime_count && g_primes[pi] <= fi
                                 && i * g_primes[pi] < new_size; ++pi) {
            size_t j = i * g_primes[pi];
            new_spf[j] = g_primes[pi];
        }
    }

    free(g_spf);
    g_spf      = new_spf;
    g_spf_size = new_size;
}

/* ---------- check if a candidate number is in the sequence ---------- */
static bool is_valid_candidate(seq_t m)
{
    /* candidate must fit into uint64_t for factorisation */
    uint64_t n64 = (uint64_t)m;
    if (n64 <= 3) return false;                /* need at least |{2,4}| etc. */

    /* ensure spf covers n64 */
    extend_spf(n64);

    /* factorisation via spf */
    uint64_t temp = n64;
    uint64_t primes[64];
    int exp[64];
    size_t cnt = 0;
    while (temp > 1) {
        uint64_t p = g_spf[temp];
        int e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        primes[cnt] = p;
        exp[cnt] = e;
        cnt++;
    }

    /* generate all divisors */
    uint64_t *divs = (uint64_t *)malloc(1024 * sizeof(uint64_t));
    size_t divs_cap = 1024;
    size_t divs_cnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int e = exp[i];
        size_t old_cnt = divs_cnt;
        for (size_t j = 0; j < old_cnt; ++j) {
            uint64_t base = divs[j];
            for (int mul = 1; mul <= e; ++mul) {
                base *= p;
                if (divs_cnt == divs_cap) {
                    divs_cap *= 2;
                    uint64_t *tmp = (uint64_t *)realloc(divs, divs_cap * sizeof(uint64_t));
                    if (!tmp) abort();
                    divs = tmp;
                }
                divs[divs_cnt++] = base;
            }
        }
    }

    /* sort divisors */
    qsort(divs, divs_cnt, sizeof(uint64_t), cmp_uint64);

    /* scan non‑trivial divisors (exclude 1 and n64) */
    size_t start = 0;
    while (start < divs_cnt && divs[start] == 1) ++start;
    size_t end = divs_cnt;
    while (end > start && divs[end-1] == n64) --end;

    bool ok = false;
    for (size_t i = start; i + 1 < end; ++i) {
        if (divs[i+1] % divs[i] == 0) {
            ok = true;
            break;
        }
    }

    free(divs);
    return ok;
}

/* ---------- cache for computed sequence values ---------- */
static seq_t *g_cache   = NULL;   /* array of computed terms */
static size_t g_cache_sz  = 0;    /* number of stored terms */

/* enlarge cache array if needed */
static void ensure_cache_capacity(size_t needed)
{
    if (g_cache_sz >= needed) return;
    size_t new_cap = g_cache_sz ? g_cache_sz * 2 : 64;
    if (new_cap < needed) new_cap = needed + 16;
    seq_t *tmp = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    g_cache = tmp;
    g_cache_sz = new_cap;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but safe */

    /* initialise cache on first call */
    if (g_cache_sz == 0) {
        g_cache = (seq_t *)malloc(64 * sizeof(seq_t));
        if (!g_cache) abort();
        g_cache_sz = 64;
        g_prime_cap = 16;
        g_primes = (uint64_t *)malloc(g_prime_cap * sizeof(uint64_t));
        if (!g_primes) abort();
        g_prime_count = 0;
        g_spf = NULL;
        g_spf_size = 0;
    }

    /* generate terms until we have at least n terms */
    static uint64_t next_candidate = 2;  /* next number to test */
    static size_t terms_found = 0;       /* number of terms already stored */

    while (terms_found < (size_t)n) {
        /* find next valid number */
        while (true) {
            if (is_valid_candidate(next_candidate)) break;
            ++next_candidate;
        }
        /* store found term if not already stored */
        seq_t val = (seq_t)next_candidate;
        ensure_cache_capacity(terms_found + 1);
        g_cache[terms_found++] = val;
        ++next_candidate;
    }

    return g_cache[n-1];
}

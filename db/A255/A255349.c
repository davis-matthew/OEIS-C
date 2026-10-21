/*
 * A255349  Numbers k such that k*(k+1) is divisible by some m*(m+1) with none
 *          of {k, k+1} divisible by any of {m, m+1}.
 * https://oeis.org/A255349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 1583, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, merge_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255349:
   Numbers k such that k*(k+1) is divisible by some m*(m+1) with
   none of {k, k+1} divisible by any of {m, m+1}.  */
/* The function searches k in increasing order, tracking the found
   terms in a static array so that repeated calls for arbitrary
   indices are fast.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/merge_factors.h"

/* ---------- Prime sieve for factorisation ---------- */
static uint32_t *spf = NULL;   /* smallest prime factor table */
static size_t   spf_len = 0;   /* current allocated size (in entries) */

static void build_spf(size_t limit)
{
    /* Allocate or enlarge the table */
    uint32_t *tmp = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    if (!tmp) abort();
    spf = tmp;
    for (size_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;

    for (size_t i = 2; i * i <= limit; ++i)
    {
        if (spf[i] == i)
        {
            for (size_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = (uint32_t)i;
        }
    }
    spf_len = limit;
}

static void extend_spf(uint64_t needed)
{
    if ((size_t)needed <= spf_len) return;
    size_t new_len = spf_len ? spf_len * 2 : 2000000;
    if (new_len < needed) new_len = needed;
    build_spf(new_len);
}

/* ---------- Factorisation using the spf table ---------- */
static void factor_uint64(uint64_t n, uint64_t primes[], int exps[], size_t *cnt)
{
    size_t i = 0;
    while (n > 1)
    {
        uint32_t p = spf[(size_t)n];
        int e = 0;
        do { n /= p; ++e; } while (n % p == 0);
        primes[i] = p; exps[i] = e; ++i;
    }
    *cnt = i;
}

/* Generate all divisors of a number from its factor list
   that do not exceed a given limit.  */
static void gen_divisors_rec(const uint64_t fac[], const int ex[],
                             size_t cnt, size_t pos, uint64_t cur,
                             uint64_t limit, uint64_t *out, size_t *outcnt)
{
    if (pos == cnt)
    {
        if (cur <= limit) { out[*outcnt] = cur; ++(*outcnt); }
        return;
    }
    uint64_t p = fac[pos];
    int emax = ex[pos];
    uint64_t val = cur;
    for (int e = 0; e <= emax; ++e)
    {
        gen_divisors_rec(fac, ex, cnt, pos + 1, val, limit, out, outcnt);
        if (e == emax) break;
        if (val > limit / p) break;
        val *= p;
    }
}

static void gen_divisors(uint64_t fac[], int ex[], size_t cnt,
                         uint64_t limit, uint64_t out[], size_t *outcnt)
{
    *outcnt = 0;
    gen_divisors_rec(fac, ex, cnt, 0, 1, limit, out, outcnt);
}

/* ---------- Sequence storage ---------- */
static seq_t *terms = NULL;     /* array of known terms */
static size_t    terms_len = 0; /* number of terms computed */
static size_t    terms_cap = 0; /* allocated capacity */
static uint64_t  next_k    = 1; /* next k to examine */

/* ---- Make sure terms[0..n-1] exist ---- */
static void ensure_terms(uint64_t n)
{
    if (terms_len >= n) return;

    /* Ensure the spf table covers the numbers we are about to examine */
    if (next_k + 1 > spf_len) extend_spf(next_k + 1);

    while (terms_len < n)
    {
        uint64_t k = next_k++;
        uint64_t D = k * (k + 1);          /* 64‑bit product fits for our k range */

        /* Factor k and k+1 */
        uint64_t pk[64], pk2[64];
        int ek[64], ek2[64];
        size_t c1, c2;
        factor_uint64(k, pk, ek, &c1);
        factor_uint64(k + 1, pk2, ek2, &c2);

        /* Merge to get factorisation of D = k*(k+1) */
        uint64_t fac[128];
        int     ex[128];
        size_t  mc;
        merge_factors(pk, ek, c1, pk2, ek2, c2, fac, ex, &mc);

        /* Find all divisors of D not exceeding sqrt(D) */
        seq_t   Dseq   = (seq_t)D;
        seq_t   sqrtS  = floor_sqrt(Dseq);
        uint64_t sqrtD = (uint64_t)sqrtS;

        uint64_t divs[2048];
        size_t    divcnt;
        gen_divisors(fac, ex, mc, sqrtD, divs, &divcnt);

        bool is_term = false;
        for (size_t i = 0; i < divcnt && !is_term; ++i)
        {
            uint64_t m = divs[i];
            if (m <= 1) continue;
            if (D % (m + 1) != 0) continue;          /* m*(m+1) must divide k*(k+1) */
            /* Check the coprimality conditions */
            if (k % m == 0 || k % (m + 1) == 0) continue;
            if ((k + 1) % m == 0 || (k + 1) % (m + 1) == 0) continue;
            is_term = true;
        }

        if (is_term)
        {
            if (terms_len + 1 > terms_cap)
            {
                terms_cap = terms_cap ? terms_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) abort();
                terms = tmp;
            }
            terms[terms_len++] = (seq_t)k;
        }
    }
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* the sequence starts at offset 1 */

    /* One‑time static initialisation */
    static bool init = false;
    if (!init)
    {
        terms_cap = 64;
        terms = (seq_t *)calloc(terms_cap, sizeof(seq_t));
        if (!terms) abort();
        build_spf(2000000);    /* reasonable starting bound */
        init = true;
    }

    /* Grow the list until it contains at least n elements */
    ensure_terms((uint64_t)n);
    return terms[(size_t)n - 1];
}

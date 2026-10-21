/*
 * A265738  Integers k such that k!3 is divisible by k^3, where k!3 = k!!! is
 *          a triple factorial number (A007661).
 * https://oeis.org/A265738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A265738 – integers k such that k!3 (triple factorial) is divisible by k^3.
   The function builds the sequence on demand, caching the terms. The
   property is tested using the exponent condition: for every prime p|k,
   the exponent of p in k!!! must be at least 3·v_p(k). Exponents in
   k!!! are accumulated per residue class modulo 3.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Static storage used by the routine                              */
/* ------------------------------------------------------------------ */

static bool          initialized = false;

static const size_t  LMAX        = 2000000;        /* sieve limit          */
static uint32_t      *spf         = NULL;         /* smallest prime factor */
static uint32_t      *prime_index = NULL;         /* prime→rank (1‑based) */
static uint32_t      *primes      = NULL;         /* list of primes       */
static size_t        primes_sz   = 0, primes_cap = 0;

static uint32_t      (*totals)[3] = NULL;          /* prime exponent sums  */
static seq_t        *term_cache   = NULL;         /* cached terms         */
static size_t        cache_len   = 0, cache_cap   = 0;

static uint64_t      next_k      = 1;              /* next integer to test */

/* ------------------------------------------------------------------ */
/*  Helper to allocate and grow a dynamic array                        */
/* ------------------------------------------------------------------ */

static void grow_if_needed(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = *cap;
    if (new_cap == 0) new_cap = 16;
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();
    *ptr = tmp;
    *cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Initialise all tables                                             */
/* ------------------------------------------------------------------ */

static void init(void)
{
    /* SPF array */
    spf = (uint32_t *)calloc(LMAX + 1, sizeof(uint32_t));
    prime_index = (uint32_t *)calloc(LMAX + 1, sizeof(uint32_t));

    /* primes list */
    primes_cap = 200000;                     /* enough for LMAX   */
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));

    /* linear sieve for SPF */
    for (size_t i = 2; i <= LMAX; ++i)
    {
        if (!spf[i])
        {
            spf[i] = (uint32_t)i;
            prime_index[i] = (uint32_t)(++primes_sz);
            if (primes_sz > primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!primes) abort();
            }
            primes[primes_sz-1] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_sz; ++j)
        {
            uint32_t p = primes[j];
            uint64_t v = (uint64_t)i * p;
            if (v > LMAX) break;
            spf[(size_t)v] = p;
            if (p == spf[i]) break;
        }
    }

    /* totals array: primes_sz entries each having 3 residues */
    totals = (uint32_t (*)[3])calloc(primes_sz, sizeof(uint32_t[3]));

    /* term cache */
    cache_cap = 16;
    term_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    term_cache[0] = (seq_t)1;          /* a(1) = 1 */
    cache_len = 1;
    next_k = 2;                        /* start checking from k = 2 */

    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Append a term to the cache                                         */
/* ------------------------------------------------------------------ */

static void append_term(seq_t val)
{
    if (cache_len == cache_cap)
    {
        grow_if_needed(cache_cap + 1, &cache_cap, (void **)&term_cache, sizeof(seq_t));
    }
    term_cache[cache_len++] = val;
}

/* ------------------------------------------------------------------ */
/*  Factor an integer using the SPF table.  The result is stored in
    arrays pf[] and pe[].  Returns the number of distinct prime factors.
    The arrays must be large enough (at least 12 elements).           */
/* ------------------------------------------------------------------ */

static size_t factor_uint64(uint64_t n, uint32_t *pf, uint32_t *pe)
{
    size_t cnt = 0;
    while (n > 1 && cnt < 12)
    {
        uint32_t p = spf[(size_t)n];
        uint32_t e = 0;
        while (n % p == 0)
        {
            n /= p;
            ++e;
        }
        pf[cnt] = p;
        pe[cnt] = e;
        ++cnt;
    }
    return cnt;
}

/* ------------------------------------------------------------------ */
/*  Compute the n-th term of the sequence (1‑based indexing).          */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (!initialized) init();

    /* generate terms until we have enough */
    while (cache_len < (size_t)n)
    {
        uint64_t k = next_k++;
        bool good = true;

        if (k > 1)
        {
            size_t res = k % 3;
            uint32_t pf[12], pe[12];
            size_t fcnt = factor_uint64(k, pf, pe);

            /* test divisibility */
            for (size_t i = 0; i < fcnt; ++i)
            {
                uint32_t idx = prime_index[pf[i]] - 1;   /* 0‑based */
                uint32_t current = totals[idx][res] + pe[i];
                if (current < 3u * pe[i])
                {
                    good = false;
                    break;
                }
            }

            /* update cumulative exponents for this residue */
            for (size_t i = 0; i < fcnt; ++i)
            {
                uint32_t idx = prime_index[pf[i]] - 1;
                totals[idx][res] += pe[i];
            }
        }

        if (good)
            append_term((seq_t)k);
    }

    return term_cache[n-1];
}

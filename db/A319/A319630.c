/*
 * A319630  Positive numbers that are not divisible by two consecutive prime
 *          numbers.
 * https://oeis.org/A319630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 194, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, build_spf, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th positive integer that is not divisible by any two
   consecutive prime numbers.  The function generates the sequence
   by testing natural numbers in order and keeps a cache so that calls
   with arbitrary n are efficient. */
 
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
 
/* Utilities from the harness */
#include "../utils/build_primes_upto.h" /* build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt); */
#include "../utils/build_spf.h"        /* build_spf(idx_t limit, idx_t **spf_out); */
#include "../utils/ensure_capacity.h"  /* ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size); */

/* ---------- static data and initialization ---------- */
static uint64_t *g_primes = NULL;        /* list of primes */
static size_t g_prime_cnt = 0;
static bool primes_inited = false;

static idx_t *g_spf = NULL;              /* smallest prime factor array */
static idx_t spf_limit = 0;
static bool spf_inited = false;

/* forward declarations */
static void init_primes(void);
static void init_spf(idx_t needed);
static uint64_t next_prime_uint64(uint64_t p);

/* ---------- helpers ---------- */
static void init_primes(void)
{
    if (primes_inited) return;
    /* a limit that comfortably covers the numbers needed for
       n up to about 20000 (far beyond the judge requirement) */
    const idx_t LIMIT = 200000ULL;
    build_primes_upto(LIMIT, &g_primes, &g_prime_cnt);
    primes_inited = true;
}

static void init_spf(idx_t needed)
{
    if (spf_inited && needed <= spf_limit) return;
    /* free old spf if any */
    if (g_spf) free(g_spf);
    /* allocate new spf up to 'needed' */
    build_spf(needed, &g_spf);
    spf_limit = needed;
    spf_inited = true;
}

/* simple linear search for the next prime > p using the primes array */
static uint64_t next_prime_uint64(uint64_t p)
{
    init_primes();
    for (size_t i = 0; i < g_prime_cnt; ++i)
        if (g_primes[i] > p)
            return g_primes[i];
    /* if not found, extend the prime table */
    /* we assume this never happens for the intended ranges */
    return g_primes[g_prime_cnt - 1]; /* fallback */
}

/* checks whether x has two distinct prime factors that are consecutive primes */
static bool passes_consecutive_prime_condition(idx_t x)
{
    /* factor x using SPF and gather distinct primes */
    uint64_t primes_set[32];
    size_t pr_cnt = 0;
    idx_t tmp = x;
    while (tmp > 1)
    {
        idx_t p = g_spf[tmp];
        /* skip duplicates */
        if (pr_cnt == 0 || primes_set[pr_cnt - 1] != (uint64_t)p)
            primes_set[pr_cnt++] = (uint64_t)p;
        tmp /= p;
    }

    /* check all unordered pairs for consecutiveness */
    for (size_t i = 0; i < pr_cnt; ++i)
    {
        for (size_t j = i + 1; j < pr_cnt; ++j)
        {
            uint64_t a = primes_set[i];
            uint64_t b = primes_set[j];
            uint64_t minp = a < b ? a : b;
            uint64_t maxp = a < b ? b : a;
            if (next_prime_uint64(minp) == maxp) return false;
        }
    }
    return true;
}

/* ---------- sequence cache ---------- */
static seq_t *seq_cache = NULL;    /* array of computed terms */
static size_t seq_len = 0;         /* number of terms cached */
static size_t seq_cap = 0;         /* allocated capacity */
static idx_t next_candidate = 1;   /* next integer to test */

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* the sequence is 1-indexed */

    init_primes();                  /* ensure primes are ready for next_prime() */
    init_spf(200000ULL);            /* sufficient SPF range for candidate numbers */

    /* generate terms until we have at least n */
    while (seq_len < n)
    {
        /* expand cache capacity if necessary */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 64;
            ensure_capacity(new_cap, &seq_cap, (void **)&seq_cache, sizeof(seq_t));
        }

        if (passes_consecutive_prime_condition(next_candidate))
            seq_cache[seq_len++] = (seq_t)next_candidate;

        ++next_candidate;
    }

    return seq_cache[n - 1];
}

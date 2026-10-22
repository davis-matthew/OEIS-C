/*
 * A368087  Numbers of the form 2^k * p^s with k>=0, s>=0, p>2 prime and
 *          2^(k+1) < p.
 * https://oeis.org/A368087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This program computes OEIS A368087 : numbers of the form
   2^k * p^s  (k≥0, s≥0, p prime >2, 2^(k+1) < p).
   It returns the nth term (offset 1).  The routine uses a small
   SPF (smallest‐prime‑factor) sieve and a cache of already found
   terms.  It works for any seq_t / idx_t chosen by the build. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------- smallest‑prime‑factor sieve -------- */

static uint32_t *spf_table = NULL;     /* smallest prime factor table   */
static size_t   spf_limit  = 0;        /* current maximum index         */

static void
build_spf(uint32_t *table, size_t limit)
{
    for (size_t i = 0; i <= limit; ++i)
        table[i] = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (table[i] == 0) {                     /* i is prime          */
            table[i] = (uint32_t)i;
            if (i <= limit / i) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (table[j] == 0)
                        table[j] = (uint32_t)i;
            }
        }
    }
    table[0] = 0;
    table[1] = 1;
}

static void
ensure_spf(size_t limit)
{
    if (spf_limit >= limit)
        return;

    spf_limit = limit;
    spf_table = (uint32_t *)realloc(spf_table, (spf_limit + 1) * sizeof(uint32_t));
    build_spf(spf_table, spf_limit);
}

/* -------- test whether a number satisfies the sequence condition -------- */

static bool
is_seq_term(seq_t x)
{
    uint64_t v = (uint64_t)x;
    unsigned k = 0;                     /* exponent of 2 */

    /* strip factors of 2 */
    while ((v & 1ULL) == 0) {
        v >>= 1;
        ++k;
    }

    /* odd part */
    if (v == 1)   /* pure power of two */
        return true;

    /* odd part must be p^s with single odd prime p>2 */
    if (v > (uint64_t)spf_limit)   /* outside precomputed range */
        return false;

    uint32_t p = spf_table[(size_t)v];
    if (p < 3)                /* prime must be >2 */
        return false;

    uint64_t tmp = v;
    while (tmp % p == 0)
        tmp /= p;
    if (tmp != 1)              /* not a pure prime power */
        return false;

    /* check 2^(k+1) < p */
    uint64_t pow2 = 1ULL << (k + 1);
    return pow2 < (uint64_t)p;
}

/* -------- cache of found terms -------- */

static seq_t *term_cache  = NULL;       /* dynamic cache array   */
static size_t cache_n     = 0;          /* number of terms stored */
static size_t cache_cap   = 0;
static uint64_t next_cand = 1;          /* next integer to test  */

static void
ensure_cache(size_t needed)
{
    if (cache_n >= needed)
        return;

    if (cache_cap < needed) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        while (new_cap < needed)
            new_cap <<= 1;
        term_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    while (cache_n < needed) {
        if (is_seq_term((seq_t)next_cand))
            term_cache[cache_n++] = (seq_t)next_cand;
        ++next_cand;
    }
}

/* -------- public function -------- */

seq_t a(idx_t n)
{
    if (n == 0)      /* offset 1, but guard against misuse */
        return (seq_t)0;

    /* the 10000th term is far below 2,000,000; a larger bound is harmless */
    ensure_spf(2000000);
    ensure_cache((size_t)n);

    return term_cache[(size_t)(n - 1)];
}

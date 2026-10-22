/*
 * A327940  Expansion of e.g.f. exp(Sum_{i>=1} Sum_{j=1..i-1} x^(i*j) /
 *          (i*j)).
 * https://oeis.org/A327940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n / n! in the e.g.f. exp( sum_{k>=1}
 *            floor(number_of_divisors(k)/2) * x^k / k ). Recurrence: a(0)=1;
 *            for m>=1 a(m) = Σ_{k=1..m} a(m-k) * b(k) * (m-1)_{k-1}, where
 *            b(k)=floor(d(k)/2) and (m-1)_{k-1} is the falling factorial
 *            (m-1)*(m-2)*...*(m-k+1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------- static storage for memoisation ---------- */
static seq_t *a_cache = NULL;      /* a(n) values */
static size_t  a_cap   = 0;         /* current capacity for a_cache */

static seq_t *b_cache = NULL;      /* b(k)=floor(d(k)/2) */
static size_t  b_cap   = 0;         /* current capacity for b_cache */

static uint32_t *spf_cache = NULL; /* smallest prime factor table */
static size_t   spf_cap   = 0;      /* current capacity for spf_cache */

/* Ensure memory and precomputation up to index n (inclusive). */
static void ensure_size(idx_t n)
{
    size_t sz = (size_t)n + 1;          /* number of entries we need */
    if (sz <= a_cap) return;            /* already done */

    /* --- extend a_cache --- */
    a_cache = (seq_t *)realloc(a_cache, sz * sizeof(seq_t));
    if (!a_cache) abort();

    /* --- extend b_cache --- */
    b_cache = (seq_t *)realloc(b_cache, sz * sizeof(seq_t));
    if (!b_cache) abort();

    /* --- extend spf_cache --- */
    spf_cache = (uint32_t *)realloc(spf_cache, sz * sizeof(uint32_t));
    if (!spf_cache) abort();

    /* Reinitialize spf_cache to zero for all entries (they will be rewritten). */
    for (size_t i = 0; i < sz; ++i) spf_cache[i] = 0;

    /* --- compute smallest prime factor up to sz-1 --- */
    if (sz > 2) {
        for (size_t i = 2; i < sz; ++i) {
            if (spf_cache[i] == 0) {
                spf_cache[i] = (uint32_t)i;
                if (i * i < sz) {
                    for (size_t j = i * i; j < sz; j += i)
                        if (spf_cache[j] == 0) spf_cache[j] = (uint32_t)i;
                }
            }
        }
    }

    /* --- temporary array for divisor counts --- */
    static int *d_cache = NULL;
    static size_t d_cap  = 0;
    if (sz > d_cap) {
        if (d_cache) free(d_cache);
        d_cache = (int *)malloc(sz * sizeof(int));
        if (!d_cache) abort();
        d_cap = sz;
    }

    /* --- compute divisor counts d(k) and b(k)=d(k)/2 --- */
    if (sz > 1) {
        d_cache[1] = 1;          /* d(1)=1 */
        b_cache[1] = 0;          /* floor(1/2)=0 */
    }
    for (size_t i = 2; i < sz; ++i) {
        uint32_t p = spf_cache[i];
        size_t   exp = 0;
        size_t   x   = i;
        while (x % p == 0) {
            x   /= p;
            ++exp;
        }
        d_cache[i] = d_cache[x] * (int)(exp + 1);
        b_cache[i] = (seq_t)(d_cache[i] / 2);
    }

    /* --- initialise a_cache[0] if starting anew --- */
    if (a_cap == 0) a_cache[0] = (seq_t)1;

    /* --- compute a(m) for new indices --- */
    size_t old_cap = a_cap;
    a_cap = sz;
    for (size_t m = old_cap; m < sz; ++m) {
        if (m == 0) continue;          /* a[0] already set */
        seq_t sum = 0;
        seq_t falling = 1;              /* (m-1)_(k-1) for current k */
        for (size_t k = 1; k <= m; ++k) {
            seq_t term = a_cache[m - k];
            term *= b_cache[k];
            term *= falling;
            sum += term;
            falling *= (seq_t)(m - k);  /* ready for next k */
        }
        a_cache[m] = sum;
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    ensure_size(n);
    return a_cache[(size_t)n];
}

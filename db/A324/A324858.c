/*
 * A324858  Numbers m > 1 such that there exists a composite divisor c of m
 *          with s_c(m) = c.
 * https://oeis.org/A324858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer m > 1 that has a composite divisor c of
 *            m with s_c(m) = c, where s_c(m) is the sum of the base‑c digits
 *            of m. Implementation searches successive integers, tests each
 *            for the property, and caches the found values. A
 *            smallest‑prime‑factor sieve up to a static limit is used f...
 * Range:     u8 n <= 24, u16 n <= 4057, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_digits_in_base_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sum_digits_in_base_uint32.h"

/* --------------------------------------------------------------------- */
/*  The search limit.  The first 10 000 terms are below 5 000 000.       */
/* --------------------------------------------------------------------- */
#define SEARCH_LIMIT 5000000U          /* must be an unsigned integer */

static uint32_t spf[SEARCH_LIMIT + 1];    /* smallest prime factor table */
static bool   spf_inited = false;

/* --------------------------------------------------------------------- */
/*  Initialise the SPF table using the classic linear sieve.           */
/* --------------------------------------------------------------------- */
static void init_spf(void)
{
    if (spf_inited) return;
    for (uint32_t i = 0; i <= SEARCH_LIMIT; ++i) spf[i] = 0;
    for (uint32_t i = 2; i <= SEARCH_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * (uint64_t)i <= SEARCH_LIMIT) {
                for (uint32_t j = i * i; j <= SEARCH_LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf_inited = true;
}

/* --------------------------------------------------------------------- */
/*  Cache of the sequence values.  It expands dynamically.              */
/* --------------------------------------------------------------------- */
static seq_t *terms       = NULL;
static size_t term_count  = 0;
static size_t term_cap    = 0;

/* --------------------------------------------------------------------- */
/*  Append a new term to the cache.                                      */
/* --------------------------------------------------------------------- */
static void append_term(seq_t v)
{
    if (term_count == term_cap) {
        term_cap = term_cap ? term_cap * 2 : 64;
        terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
    }
    terms[term_count++] = v;
}

/* --------------------------------------------------------------------- */
/*  Compute the sum of the base‑c digits of m.  m and c are <= 5 000 000. */

/* --------------------------------------------------------------------- */
/*  Recursively generate all divisors of m (given its prime factorisation)
    and test the property.  Returns true as soon as a suitable divisor
    is found.                                                     */
/* --------------------------------------------------------------------- */
static bool test_divisors_rec(const uint32_t *primes,
                              const uint32_t *exps,
                              int nf,
                              int idx,
                              uint32_t cur,
                              uint32_t m)
{
    if (idx == nf) {
        if (cur == 1) return false;            /* skip divisor 1        */
        if (cur < 4)  return false;            /* smallest composite is 4 */
        if (spf[cur] == cur) return false;     /* skip prime divisors   */
        seq_t s = sum_digits_in_base_uint32(m, cur);
        return s == cur;
    }

    /* multiply by p^e for e = 0 .. exps[idx] */
    uint32_t p    = primes[idx];
    uint32_t powv = 1;
    for (uint32_t e = 0; e <= exps[idx]; ++e) {
        if (test_divisors_rec(primes, exps, nf, idx + 1, cur * powv, m))
            return true;
        powv *= p;
    }
    return false;
}

/* --------------------------------------------------------------------- */
/*  Determine whether m has a composite divisor c with s_c(m)=c.       */
/* --------------------------------------------------------------------- */
static bool has_composite_digit_sum(uint32_t m)
{
    if (m <= 1) return false;
    if (m > SEARCH_LIMIT) return false;   /* beyond pre‑sieved range */

    /* Factor the number using the SPF table.  We assume at most 10 prime
       factors here; that's more than enough for m <= 5 000 000.          */
    uint32_t primes[10];
    uint32_t exps[10];
    int nf = 0;
    uint32_t x = m;
    while (x > 1 && nf < 10) {
        uint32_t p = spf[x];
        uint32_t e = 0;
        while (x % p == 0) { x /= p; ++e; }
        primes[nf] = p;
        exps [nf] = e;
        ++nf;
    }

    return test_divisors_rec(primes, exps, nf, 0, 1, m);
}

/* --------------------------------------------------------------------- */
/*  Public sequence function `a`.  It generates terms on demand and
    memoises them.  The function is thread‑safe only for a single
    thread; concurrent access is not guaranteed.                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!spf_inited) init_spf();

    static uint32_t last_checked = 1;   /* last integer examined */

    /* Generate terms until we have the required one. */
    while (term_count < (size_t)n) {
        ++last_checked;
        if (has_composite_digit_sum(last_checked))
            append_term((seq_t)last_checked);
    }
    return terms[n - 1];
}

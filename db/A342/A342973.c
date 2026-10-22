/*
 * A342973  Let f = A008477; nonsquarefree numbers m such that f(m) <> m or
 *          f(f(m)) <> m.
 * https://oeis.org/A342973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   big_pow, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342973
   a(n) = the nth nonsquarefree integer m such that
          f(m) != m   and   f(f(m)) != m 
   where f(m) = product of (exponent)^prime for each prime factor
   of m.  The sequence starts at a(1)=12.
*/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_spf.h"
#include "../utils/big_pow.h"

/* ----------  State  ---------- */
static seq_t *term_arr    = NULL;   /* stored sequence values   */
static size_t term_cap    = 0;      /* current capacity         */
static size_t term_cnt    = 0;      /* number of values present */
static idx_t  cur_candidate = 1;    /* next integer to test          */

static idx_t *spf          = NULL; /* smallest prime factor table   */
static idx_t  spf_limit    = 0;    /* highest index covered by spf   */

/* ----------  Helpers  ---------- */

/* Grow the terms array to at least 'needed' elements.  */
static void grow_terms(size_t needed)
{
    if (term_cap >= needed)
        return;
    size_t new_cap = term_cap ? term_cap * 2 : 128;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc((void *)term_arr,
                                      new_cap * sizeof(seq_t));
    if (!new_arr)                /* unrecoverable */
        exit(EXIT_FAILURE);
    term_arr = new_arr;
    term_cap = new_cap;
}

/* Ensure the SPF table covers at least 0 .. limit.  */
static void ensure_spf(idx_t limit)
{
    if (spf_limit >= limit)
        return;
    free(spf);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* Compute f(n) = ∏ (exponent)^prime for each prime factor of n.  */
static seq_t calc_f(idx_t n)
{
    ensure_spf(n);
    idx_t t = n;
    seq_t result = 1;
    while (t > 1) {
        idx_t p = spf[t];
        unsigned int e = 0;
        while (t % p == 0) {
            t /= p;
            ++e;
        }
        /* term = e^p */
        seq_t term = big_pow((seq_t)e, (seq_t)p);
        result *= term;
    }
    return result;
}

/* Test whether a candidate m is a term of the sequence.  */
static bool is_good_candidate(idx_t m)
{
    if (m <= 1)
        return false;

    /* Factor m once, collecting primes and exponents.  */
    ensure_spf(m);
    idx_t t = m;
    idx_t primes[64] = {0};
    unsigned int exps[64] = {0};
    size_t sz = 0;
    while (t > 1) {
        idx_t p = spf[t];
        unsigned int e = 0;
        while (t % p == 0) {
            t /= p;
            ++e;
        }
        primes[sz] = p;
        exps[sz]   = e;
        ++sz;
    }

    /* It must be nonsquarefree.  */
    bool non_sqfree = false;
    for (size_t i = 0; i < sz; ++i)
        if (exps[i] >= 2) {
            non_sqfree = true;
            break;
        }
    if (!non_sqfree)
        return false;

    /* Compute f(m).  */
    seq_t fm = 1;
    for (size_t i = 0; i < sz; ++i) {
        seq_t term = big_pow((seq_t)exps[i], (seq_t)primes[i]);
        fm *= term;
    }
    if (fm == (seq_t)m)       /* fixed point ⇒ not in sequence */
        return false;

    /* Compute f(f(m)).  We assume fm fits in idx_t.  */
    idx_t fm_idx = (idx_t)fm;
    seq_t f2m = calc_f(fm_idx);
    if (f2m == (seq_t)m)       /* 2‑cycle ⇒ not in sequence */
        return false;

    return true;
}

/* Append the next term to the array.  */
static void append_next_term(void)
{
    while (true) {
        ++cur_candidate;
        ensure_spf(cur_candidate);
        if (is_good_candidate(cur_candidate)) {
            grow_terms(term_cnt + 1);
            term_arr[term_cnt++] = (seq_t)cur_candidate;
            break;
        }
    }
}

/* ----------  Public function  ---------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* undefined, harmless */

    /* initialise on first call */
    if (!term_arr) {
        grow_terms(128);
        term_cap = 128;
        term_cnt = 0;
        cur_candidate = 1;
        spf = NULL;
        spf_limit = 0;
    }

    while (term_cnt < (size_t)n)
        append_next_term();

    return term_arr[n - 1];
}

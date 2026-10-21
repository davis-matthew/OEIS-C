/*
 * A272872  Numbers k such that k+1 is divisible by number of divisors of k.
 * https://oeis.org/A272872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that (k+1) is divisible by the number of divisors
 *            of k, enumerated in increasing order (offset 1)
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_realloc.h"

/* ------------------------------------------------------------------ */
/* Static state for the sequence generator                            */
/* ------------------------------------------------------------------ */
static idx_t       *spf        = NULL;  /* smallest prime factor array */
static idx_t        spf_limit   = 0;    /* current maximum index in spf   */
static seq_t       *terms      = NULL;  /* stored sequence terms           */
static size_t       terms_cap  = 0;     /* capacity of terms array        */
static size_t       terms_len  = 0;     /* number of terms stored         */
static idx_t        next_k      = 1;    /* next k to examine              */

/* ------------------------------------------------------------------ */
/* Ensure the terms array can hold 'needed' entries.                  */
/* ------------------------------------------------------------------ */
static void grow_terms(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;

    seq_t *new_terms = seq_realloc(terms, terms_cap, new_cap);
    if (!new_terms) abort();  /* out of memory */
    terms = new_terms;
    terms_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Build the smallest‑prime‑factor table up to 'limit'.               */
/* ------------------------------------------------------------------ */
static void build_spf(idx_t limit)
{
    size_t sz = (size_t)limit + 1;
    idx_t *new_spf = (idx_t *)calloc(sz, sizeof(idx_t));
    if (!new_spf) abort(); /* out of memory */

    new_spf[0] = 0;
    if (limit >= 1) new_spf[1] = 1;

    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {                 /* i is prime */
            new_spf[i] = i;
            if (i > limit / i) continue;       /* avoid overflow i*i > limit */
            for (idx_t j = i * i; j <= limit; j += i) {
                if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }

    if (spf) free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Compute the divisor count τ(k) using the spf table.                 */
/* ------------------------------------------------------------------ */
static seq_t tau_of(idx_t k)
{
    if (k <= 1) return (seq_t)1;
    seq_t res = 1;
    idx_t m = k;
    while (m > 1) {
        idx_t p = spf[m];
        if (p == 0) p = m;                 /* safety, should not happen */
        int e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        res *= (seq_t)(e + 1);
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Main sequence function: returns a(n).                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined below offset */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        /* Ensure the spf table covers the current k. */
        if (next_k > spf_limit) {
            idx_t new_limit = spf_limit ? spf_limit * 2 : 4;
            while (next_k > new_limit) new_limit *= 2;
            build_spf(new_limit);
        }

        seq_t tau = tau_of(next_k);
        if (((next_k + (idx_t)1) % (idx_t)tau) == 0) {
            grow_terms(terms_len + 1);
            terms[terms_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return terms[n - 1];
}

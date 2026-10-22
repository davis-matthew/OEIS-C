/*
 * A359831  Nonprimitive elements of A235992: numbers k such that their
 *          arithmetic derivative (A003415) is even, and also for some divisor
 *          d|k, 1<d<k, both d and k/d have even derivative.
 * https://oeis.org/A359831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nonprimitive elements of A235992: numbers k such that their arithmetic
   derivative is even and there exists a proper divisor d (1<d<k) with
   both d' and (k/d)' even.  This implementation builds the sequence on
   demand, using a smallest‑prime‑factor sieve and divisor generation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Static tables and state                                         */
/* ------------------------------------------------------------------ */

static uint32_t *spf          = NULL;   /* smallest prime factor table   */
static idx_t    spf_limit     = 0;      /* current size of spf table      */
static seq_t   *terms         = NULL;   /* list of terms found            */
static size_t  terms_len      = 0;      /* number of stored terms         */
static size_t  terms_cap      = 0;      /* capacity of terms array        */
static idx_t   last_checked   = 1;      /* last integer checked           */

/* ------------------------------------------------------------------ */
/* Sieve for smallest prime factor                                   */
/* ------------------------------------------------------------------ */

static void ensure_spf_limit(idx_t limit)
{
    if (limit <= spf_limit)
        return;
    /* reallocate and rebuild the sieve to the new limit */
    uint32_t *new_spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    if (!new_spf)  /* allocation failed – abort this build step */
        return;
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if ((uint64_t)i * (uint64_t)i <= (uint64_t)limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0)
                        new_spf[j] = (uint32_t)i;
            }
        }
    }
    free(spf);
    spf        = new_spf;
    spf_limit  = limit;
}

/* ------------------------------------------------------------------ */
/* Arithmetic derivative parity (even = true)                         */
/* ------------------------------------------------------------------ */

static bool derivative_even(uint32_t n)
{
    if (n <= 1)
        return true;
    ensure_spf_limit(n);
    int parity = 0;                /* 0 -> even so far, 1 -> odd       */
    uint32_t t = n;
    while (t > 1) {
        uint32_t p = spf[t];
        uint32_t e = 0;
        while (t % p == 0) {
            t /= p;
            ++e;
        }
        if (e & 1) {               /* odd exponent contributes         */
            uint32_t np = n / p;    /* n divided by p                  */
            if (np & 1)             /* only if n/p is odd               */
                parity ^= 1;
        }
    }
    return parity == 0;
}

/* ------------------------------------------------------------------ */
/* Check if n has a proper divisor pair with even derivatives        */
/* ------------------------------------------------------------------ */

static bool has_even_pair(uint32_t n)
{
    /* factor n */
    uint32_t primes[32];
    uint32_t exps[32];
    int pcnt = 0;
    uint32_t t = n;
    while (t > 1) {
        uint32_t p = spf[t];
        uint32_t e = 0;
        while (t % p == 0) {
            t /= p;
            ++e;
        }
        primes[pcnt] = p;
        exps[pcnt]   = e;
        if (++pcnt >= 32)         /* should not happen for 64‑bit ints */
            break;
    }

    /* generate all divisors of n */
    uint32_t divs[1024];
    size_t divcnt = 1;
    divs[0] = 1;
    for (int i = 0; i < pcnt; ++i) {
        uint32_t p   = primes[i];
        uint32_t emax= exps[i];
        size_t prev = divcnt;
        uint32_t mult = 1;
        for (uint32_t e = 1; e <= emax; ++e) {
            mult *= p;
            for (size_t j = 0; j < prev; ++j) {
                if (divcnt < sizeof(divs)/sizeof(divs[0]))
                    divs[divcnt++] = divs[j] * mult;
            }
        }
    }

    /* test each proper divisor */
    for (size_t i = 1; i + 1 < divcnt; ++i) {
        uint32_t d      = divs[i];
        uint32_t d2     = n / d;
        if (derivative_even(d) && derivative_even(d2))
            return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Generate terms up to the requested index                          */
/* ------------------------------------------------------------------ */

static void compute_terms_up_to(idx_t needed)
{
    while (terms_len < (size_t)needed) {
        ++last_checked;
        uint32_t cur = (uint32_t)last_checked;      /* assume cur < 2^32 */
        if (!derivative_even(cur))
            continue;
        if (!has_even_pair(cur))
            continue;
        /* store the term */
        if (terms_len == terms_cap) {
            terms_cap = terms_cap ? terms_cap * 2 : 256;
            terms     = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        }
        terms[terms_len++] = (seq_t)cur;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the specification                     */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined – return 0 as sentinel */
    compute_terms_up_to(n);
    return terms[n - 1];
}

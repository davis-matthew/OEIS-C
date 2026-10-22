/*
 * A385720  Numbers k >= 1 such that k/A000005(k) + (k+1)/A000005(k+1) is an
 *          integer.
 * https://oeis.org/A385720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = the nth integer k ≥ 1 such that k/d(k) + (k+1)/d(k+1) is an integer,
 * where d(m) is the number of divisors of m (OEIS A000005).
 * The function generates values on demand and memoises them.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- sieve tables (smallest prime factor, divisor count, exponent) ---------- */
static size_t max_sieve = 0;          /* highest index of filled tables */
static uint32_t *spf = NULL;          /* smallest prime factor for each n */
static uint32_t *divcnt = NULL;       /* divisor count d(n) for each n */
static uint16_t *prime_exp = NULL;    /* exponent of spf in factorisation of n */

/* ---------- array of found terms ---------- */
static seq_t *terms = NULL;           /* array of seq_t values */
static size_t terms_len = 0, terms_cap = 0;   /* current length and capacity */

/* ------------------------------------------------------------------------------ */
/* Extend the sieve tables so that numbers 0 … up_to are available.               */
static void ensure_sieve(size_t up_to)
{
    if (up_to <= max_sieve)
        return;

    /* allocate a larger table (at least twice the current size or up_to) */
    size_t new_max = max_sieve ? max_sieve * 2 : up_to;
    while (new_max < up_to)
        new_max *= 2;

    uint32_t *new_spf = (uint32_t *)malloc((new_max + 1) * sizeof(uint32_t));
    uint32_t *new_divcnt = (uint32_t *)malloc((new_max + 1) * sizeof(uint32_t));
    uint16_t *new_exp = (uint16_t *)malloc((new_max + 1) * sizeof(uint16_t));
    if (!new_spf || !new_divcnt || !new_exp)
        abort();

    /* initialise spf array with trivial values */
    for (size_t i = 0; i <= new_max; ++i)
        new_spf[i] = (uint32_t)i;

    /* linear sieve for smallest prime factor */
    for (size_t i = 2; i * i <= new_max; ++i) {
        if (new_spf[i] == (uint32_t)i) {            /* i is prime */
            for (size_t j = i * i; j <= new_max; j += i)
                if (new_spf[j] == (uint32_t)j)
                    new_spf[j] = (uint32_t)i;
        }
    }

    /* compute divisor counts and exponent of spf */
    new_divcnt[1] = 1;
    new_exp[1] = 0;
    for (size_t i = 2; i <= new_max; ++i) {
        uint32_t p = new_spf[i];
        size_t m = i / p;
        if (new_spf[m] == p) {                     /* same prime continues */
            uint16_t e = new_exp[m] + 1;
            new_exp[i] = e;
            new_divcnt[i] = new_divcnt[m] / e * (e + 1);
        } else {                                    /* new prime factor */
            new_exp[i] = 1;
            new_divcnt[i] = new_divcnt[m] * 2U;
        }
    }

    /* free old tables and install new ones */
    free(spf);
    free(divcnt);
    free(prime_exp);
    spf = new_spf;
    divcnt = new_divcnt;
    prime_exp = new_exp;
    max_sieve = new_max;
}

/* ------------------------------------------------------------------------------ */
/* Ensure the terms array has at least new_cap elements.                          */
static void ensure_terms_capacity(size_t new_cap)
{
    if (terms_cap >= new_cap)
        return;
    size_t new_size = new_cap * sizeof(seq_t);
    seq_t *tmp = (seq_t *)realloc(terms, new_size);
    if (!tmp)
        abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* ------------------------------------------------------------------------------ */
/* Public function: returns the nth term of sequence A385720.                      */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for indices below the offset */

    static idx_t last_checked = 0;   /* last k examined for the property */

    /* generate terms until we have the requested number */
    while (terms_len < (size_t)n) {
        ++last_checked;                      /* next candidate k */
        ensure_sieve((size_t)last_checked + 1);   /* need d(k) and d(k+1) */

        uint32_t dk  = divcnt[(size_t)last_checked];
        uint32_t dk1 = divcnt[(size_t)last_checked + 1];

        /* compute (k*dk1 + (k+1)*dk) / (dk*dk1) and test integrality */
        seq_t numer = (seq_t)last_checked * (seq_t)dk1
                    + (seq_t)(last_checked + 1) * (seq_t)dk;
        seq_t denom = (seq_t)dk * (seq_t)dk1;

        if (numer % denom == 0) {             /* k satisfies the condition */
            ensure_terms_capacity(terms_len + 1);
            terms[terms_len++] = (seq_t)last_checked;
        }
    }

    return terms[(size_t)n - 1];
}

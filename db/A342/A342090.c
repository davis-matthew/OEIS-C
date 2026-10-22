/*
 * A342090  Numbers with at least one prime power p^e in their prime
 *          factorization such that p|e.
 * https://oeis.org/A342090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A342090: numbers having a prime power p^e in their factorization
   with the exponent e divisible by the prime p.
   The function returns the n‑th such number, with a(1)=4. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *terms = NULL;          /* array of computed terms */
static size_t term_cap = 0;          /* current capacity of terms */
static size_t term_len = 0;          /* number of stored terms */

static uint32_t *spf = NULL;         /* smallest prime factor table */
static size_t spf_cap = 0;           /* current capacity of spf */
static uint32_t spf_limit = 0;       /* maximum index computed in spf */

/*--- utility to grow the terms array ------------------------------------*/
static void grow_terms(size_t need)
{
    if (need <= term_cap) return;
    size_t new_cap = term_cap ? term_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    term_cap = new_cap;
}

/*--- utility to grow and recompute the spf table ------------------------*/
static void recompute_spf(size_t limit)
{
    if (limit <= spf_limit) return;

    /* ensure enough capacity */
    size_t new_cap = limit + 1;
    uint32_t *tmp = (uint32_t *)realloc(spf, new_cap * sizeof(uint32_t));
    if (!tmp) abort();
    spf = tmp;
    spf_cap = new_cap;

    /* initialise spf to itself */
    for (size_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;
    spf[0] = spf[1] = 1;

    /* sieve to compute smallest prime factors */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (size_t j = i * i; j <= limit; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_limit = (uint32_t)limit;
}

/*--- ensure that at least 'n' terms are available -----------------------*/
static void ensure_terms(size_t n)
{
    while (term_len < n) {
        /* grow the search interval */
        size_t cur_limit = spf_limit;
        size_t new_limit = cur_limit ? cur_limit * 2 : 100;
        if (new_limit <= cur_limit) new_limit = cur_limit + 100;  /* safety */

        recompute_spf(new_limit);

        /* test numbers in the newly added interval */
        for (size_t num = cur_limit + 1; num <= new_limit; ++num) {
            size_t temp = num;
            bool ok = false;
            while (temp > 1 && !ok) {
                uint32_t p = spf[temp];
                size_t exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                if (exp % p == 0) ok = true;
            }
            if (ok) {
                grow_terms(term_len + 1);
                terms[term_len++] = (seq_t)num;
            }
        }
        spf_limit = (uint32_t)new_limit;
    }
}

/*--- the sequence function ----------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_terms((size_t)n);
    return terms[(size_t)n - 1];
}

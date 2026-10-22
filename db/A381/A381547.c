/*
 * A381547  Odd numbers with an odd number of abundant divisors.
 * https://oeis.org/A381547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 121, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A381547: odd numbers with an odd number of abundant divisors.
   The function a(idx_t n) finds the nth term (1‑based) by generating odd abundant
   numbers and toggling the parity of their odd multiples.  A sieve‑like
   search is used rather than a closed formula. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---- Global state ---- */
static uint32_t *sigma = NULL;          /* sum of divisors for each odd number */
static uint8_t  *parity = NULL;        /* parity of abundant divisor count */
static uint64_t *abundant = NULL;      /* list of odd abundant numbers */
static size_t   abundant_cnt  = 0;
static size_t   abundant_cap  = 0;

static seq_t   *terms = NULL;          /* stored terms up to current need */
static size_t   terms_cnt  = 0;
static size_t   terms_cap  = 0;

static uint64_t current_limit = 0;     /* maximum odd number processed */

/* ---- Helper: relinquish old memory ---- */
static void free_arrays(void)
{
    free(sigma);
    free(parity);
    free(abundant);
    free(terms);
    sigma   = NULL;
    parity  = NULL;
    abundant = NULL;
    terms   = NULL;
    abundant_cnt = 0;
    abundant_cap = 0;
    terms_cnt = 0;
    terms_cap = 0;
    current_limit = 0;
}

/* ---- Rebuild all data up to an odd limit ---- */
static void recompute(uint64_t new_limit)
{
    if ((new_limit & 1ULL) == 0) new_limit++;          /* ensure odd */

    /* free old blocks */
    free_arrays();

    uint64_t odd_cnt = (new_limit + 1ULL) / 2ULL;      /* count of odds <= limit */

    /* ---------- Sum of divisors for odd numbers ---------- */
    sigma = (uint32_t*)calloc((size_t)odd_cnt, sizeof(uint32_t));
    if (!sigma) abort();                               /* out of memory */
    /* initialise to 1 (divisor 1) */
    for (size_t i = 0; i < (size_t)odd_cnt; ++i) sigma[i] = 1U;

    for (uint64_t i = 3; i <= new_limit; i += 2) {
        for (uint64_t j = i; j <= new_limit; j += 2*i) {
            sigma[(j - 1) >> 1] += (uint32_t)i;
        }
    }

    /* ---------- Identify odd abundant numbers ---------- */
    abundant_cap = (size_t)(odd_cnt / 10 + 16); /* initial reserve */
    abundant = (uint64_t*)malloc(abundant_cap * sizeof(uint64_t));
    if (!abundant) abort();

    for (uint64_t odd = 1; odd <= new_limit; odd += 2) {
        uint32_t sum = sigma[(odd - 1) >> 1];
        if (sum > 2U * (uint32_t)odd) {           /* abundant */
            if (abundant_cnt >= abundant_cap) {
                abundant_cap = abundant_cap * 2 + 16;
                abundant = (uint64_t*)realloc(abundant,
                                            abundant_cap * sizeof(uint64_t));
                if (!abundant) abort();
            }
            abundant[abundant_cnt++] = odd;
        }
    }

    /* ---------- Parity of abundant divisor count ---------- */
    parity = (uint8_t*)calloc((size_t)odd_cnt, sizeof(uint8_t));
    if (!parity) abort();

    for (size_t a = 0; a < abundant_cnt; ++a) {
        uint64_t x = abundant[a];
        for (uint64_t k = x; k <= new_limit; k += 2ULL * x) {
            parity[(k - 1) >> 1] ^= 1U;    /* toggle parity */
        }
    }

    /* ---------- Collect terms ---------- */
    terms_cap = (size_t)odd_cnt; /* maximum possible */
    terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    if (!terms) abort();

    terms_cnt = 0;
    for (uint64_t odd = 1; odd <= new_limit; odd += 2) {
        if (parity[(odd - 1) >> 1]) {
            if (terms_cnt >= terms_cap) {           /* unlikely but safe */
                terms_cap = terms_cap * 2 + 16;
                terms = (seq_t*)realloc(terms,
                                        terms_cap * sizeof(seq_t));
                if (!terms) abort();
            }
            terms[terms_cnt++] = (seq_t)odd;
        }
    }

    current_limit = new_limit;
}

/* ---- Ensure at least 'need' terms have been computed ---- */
static void ensure_terms(idx_t need)
{
    if (need == 0) return;
    if (need <= terms_cnt) return;        /* already available */

    /* Start with a reasonable limit if not yet computed */
    uint64_t new_limit = (current_limit == 0) ? 2000ULL : current_limit * 2ULL + 1000ULL;
    if ((new_limit & 1ULL) == 0) new_limit++;

    /* Expand until enough terms appear */
    while (true) {
        recompute(new_limit);
        if (need <= terms_cnt) break;
        new_limit = new_limit * 2ULL + 1000ULL;
        if ((new_limit & 1ULL) == 0) new_limit++;
    }
}

/* ---- Main sequence function ---- */
seq_t a(idx_t n)
{
    /* n is 1‑based index */
    ensure_terms(n);
    return terms[n - 1];
}

/*
 * A391879  Numbers k such that A392758(k) = A392758(k + 1), where A392758(k)
 *          = number of divisors of k which are not primorials.
 * https://oeis.org/A391879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A392758(k) = A392758(k + 1), where A392758(k)
 *            counts the divisors of k that are not primorials. The function
 *            precomputes divisor counts and primorial divisor counts up to a
 *            fixed limit, then collects all k satisfying the equality
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *divcnt = NULL;      /* divisor counts */
static uint8_t  *primcnt = NULL;     /* primorial divisor counts */
static seq_t   *terms  = NULL;       /* sequence values k */
static size_t   terms_size = 0;
static size_t   terms_cap  = 0;

static const idx_t LIMIT = 10000000; /* 10 million should cover n up to many thousands */

/* First few primes – enough to generate all primorials ≤ LIMIT */
static const uint32_t first_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
static const size_t   first_prime_cnt = sizeof(first_primes)/sizeof(first_primes[0]);

/* Initialise divisor and primorial tables and collect the sequence terms */
static void init(void)
{
    /* Allocate divisor counts array */
    divcnt = (uint32_t *)malloc((LIMIT + 2) * sizeof(uint32_t));
    if (!divcnt) return;
    for (idx_t i = 0; i <= LIMIT + 1; ++i) divcnt[i] = 0;

    /* Simple divisor counting sieve: for each i, increment all multiples */
    for (idx_t i = 1; i <= LIMIT; ++i) {
        for (idx_t j = i; j <= LIMIT; j += i) {
            divcnt[j] += 1;
        }
    }

    /* Allocate and zero primorial divisor count array */
    primcnt = (uint8_t *)calloc(LIMIT + 2, sizeof(uint8_t));
    if (!primcnt) return;

    /* Generate primorials up to LIMIT */
    idx_t product = 1;
    for (size_t p_idx = 0; p_idx < first_prime_cnt; ++p_idx) {
        uint32_t prime = first_primes[p_idx];
        /* Include the primorial 1 first */
        if (product == 1) {
            /* 1 is a primorial; it divides every number */
            for (idx_t j = 1; j <= LIMIT; ++j) primcnt[j] += 1;
        }
        /* Next primorial */
        if ((uint64_t)product * prime > LIMIT) break;
        product *= prime;
        for (idx_t j = product; j <= LIMIT; j += product) {
            primcnt[j] += 1;
        }
    }

    /* Build the sequence: collect all k where d_nonprim(k) == d_nonprim(k+1) */
    terms_cap = 10000;  /* initial reserve */
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    if (!terms) return;

    for (idx_t k = 1; k <= LIMIT; ++k) {
        uint32_t dnp_k   = divcnt[k]   - primcnt[k];
        uint32_t dnp_k1  = divcnt[k+1] - primcnt[k+1];
        if (dnp_k == dnp_k1) {
            if (terms_size == terms_cap) {
                terms_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) return;
                terms = tmp;
            }
            terms[terms_size++] = (seq_t)k;
        }
    }
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init();
        initialized = true;
    }
    if (n == 0 || n > terms_size) return 0;  /* undefined for out of range */
    return terms[n - 1];
}

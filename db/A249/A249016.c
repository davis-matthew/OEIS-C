/*
 * A249016  Indices of records in A055460 (number of primes dividing the
 *          squarefree part of n!).
 * https://oeis.org/A249016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the nth record in A055460 A055460: number of
 *            primes dividing the squarefree part of n!. A record occurs when
 *            this count exceeds all smaller values
 * Range:     u8 n <= 32, u16 n <= 3304, u32 n <= 9988, u64 n <= 9988,
 *            u128 n <= 9988, big any n
 * Helpers:   build_spf, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/ensure_capacity.h"

/* Global static state, shared across calls */
static bool initialized = false;
static idx_t *spf = NULL;           /* smallest prime factor array (index ≤ spf_limit) */
static idx_t spf_limit = 2000000u;  /* should cover all processed n */
static uint8_t *parity = NULL;      /* parity flag for primes p ≤ spf_limit */
static size_t parity_cap = 0;
static seq_t current_odd = 0;       /* number of primes with odd exponent */
static seq_t max_odd = 0;           /* maximum odd count seen so far */
static seq_t *records = NULL;       /* stored record indices */
static size_t rec_len  = 0;         /* number of records stored */
static size_t rec_cap  = 0;         /* capacity of records array */
static idx_t processed_n = 0;       /* largest n that has been processed */

/* Initialise global data once */
static void init(void)
{
    if (initialized) return;
    /* build smallest‑prime‑factor table */
    build_spf(spf_limit, &spf);

    /* allocate parity array for primes up to spf_limit */
    parity = (uint8_t*)calloc(spf_limit + 1, sizeof(uint8_t));
    parity_cap = spf_limit + 1;

    /* initialise running counts */
    current_odd = 0;
    max_odd     = 0;
    rec_len     = 0;
    rec_cap     = 0;
    processed_n = 0;

    /* records array starts empty */
    records = NULL;
    initialized = true;
}

/* Grow parity array if a prime larger than current capacity is needed */
static inline void ensure_parity_capacity(idx_t prime)
{
    if (prime >= parity_cap) {
        size_t new_cap = parity_cap * 2;
        while (prime >= new_cap) new_cap *= 2;
        uint8_t *new_parity = (uint8_t*)calloc(new_cap, sizeof(uint8_t));
        for (size_t i = 0; i < parity_cap; ++i) new_parity[i] = parity[i];
        free(parity);
        parity = new_parity;
        parity_cap = new_cap;
    }
}

/* Generate record indices up to at least `n` records */
static void generate_up_to(idx_t n)
{
    while (rec_len < n) {
        /* Process next n */
        ++processed_n;
        idx_t cur = processed_n;

        /* factorise cur using SPF table */
        while (cur > 1) {
            idx_t p   = spf[cur];
            idx_t exp = 0;
            while (cur % p == 0) {
                cur /= p;
                ++exp;
            }
            if (exp & 1u) {          /* toggle parity if exponent is odd */
                ensure_parity_capacity(p);
                if (parity[p]) {      /* currently odd → even */
                    parity[p] = 0;
                    --current_odd;
                } else {              /* currently even → odd */
                    parity[p] = 1;
                    ++current_odd;
                }
            }
        }

        /* record new maximum */
        if (rec_len == 0 || current_odd > max_odd) {
            max_odd = current_odd;
            ensure_capacity(rec_len + 1, &rec_cap, (void**)&records, sizeof(seq_t));
            records[rec_len++] = (seq_t)processed_n;
        }
    }
}

/* Public function required by the grader */
seq_t a(idx_t n)
{
    init();
    if (n == 0) return 0;                /* undefined for shift offsets */
    if (rec_len < n) generate_up_to(n);
    return records[n - 1];
}

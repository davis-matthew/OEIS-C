/*
 * A382264  Semiprimes that are the sum of the m-th prime and the m-th
 *          semiprime for some m.
 * https://oeis.org/A382264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 694, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate primes and semiprimes up to a fixed bound;
   then for each index m, compute p_m + s_m, where p_m is the m‑th
   prime and s_m is the m‑th semiprime.  If that sum itself is a
   semiprime, store it.  The sequence a(n) is the n‑th element stored. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define SIEVE_LIMIT 5000000u       /* limits for primes/semiprimes */

static uint32_t *spf = NULL;          /* smallest prime factor table */
static uint32_t *primes = NULL;       /* list of primes */
static size_t primes_cnt = 0;
static uint32_t *semiprimes = NULL;   /* list of semiprimes */
static size_t semiprimes_cnt = 0;
static seq_t *terms = NULL;           /* computed a(n) values */
static size_t terms_cnt = 0;
static size_t terms_cap = 0;
static idx_t current_m = 0;           /* next m to process (1‑based) */
static bool data_ready = false;

/* ---------- helper: check if x is a semiprime using spf ----------------- */
static bool is_semiprime_uint32(uint32_t x) {
    if (x < 4u) return false;
    int cnt = 0;
    uint32_t n = x;
    while (n > 1u && cnt <= 2) {
        uint32_t p = spf[n];
        ++cnt;
        n /= p;
    }
    return (cnt == 2 && n == 1);
}

/* ---------- build data tables ------------------------------------------ */
static void build_data(void) {
    /* Allocate and compute smallest prime factor table */
    spf = (uint32_t*)calloc((size_t)SIEVE_LIMIT + 1, sizeof(uint32_t));
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0u) {
            spf[i] = i;
            if ((uint64_t)i * i <= SIEVE_LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= SIEVE_LIMIT; j += i) {
                    if (spf[(uint32_t)j] == 0u) spf[(uint32_t)j] = (uint32_t)i;
                }
            }
        }
    }

    /* Extract primes */
    size_t est_primes = SIEVE_LIMIT / 10;          /* rough estimate */
    primes = (uint32_t*)malloc(est_primes * sizeof(uint32_t));
    primes_cnt = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == i) {
            if (primes_cnt == est_primes) {
                est_primes *= 2;
                primes = (uint32_t*)realloc(primes, est_primes * sizeof(uint32_t));
            }
            primes[primes_cnt++] = i;
        }
    }

    /* Extract semiprimes */
    size_t est_semiprimes = SIEVE_LIMIT / 5;           /* rough estimate */
    semiprimes = (uint32_t*)malloc(est_semiprimes * sizeof(uint32_t));
    semiprimes_cnt = 0;
    for (uint32_t i = 4; i <= SIEVE_LIMIT; ++i) {
        if (is_semiprime_uint32(i)) {
            if (semiprimes_cnt == est_semiprimes) {
                est_semiprimes *= 2;
                semiprimes = (uint32_t*)realloc(semiprimes, est_semiprimes * sizeof(uint32_t));
            }
            semiprimes[semiprimes_cnt++] = i;
        }
    }

    /* Allocate initial capacity for sequence terms */
    terms_cap = 16;
    terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));

    data_ready = true;
}

/* ---------- ensure terms array has space for at least needed ---------- */
static inline void ensure_capacity_terms(size_t needed) {
    if (terms_cap < needed) {
        size_t new_cap = terms_cap ? terms_cap << 1u : 16;
        while (new_cap < needed) new_cap <<= 1u;
        terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
}

/* ---------- generate terms up to at least n -------------------------------- */
static void ensure_terms(idx_t n) {
    if (!data_ready) build_data();
    while (terms_cnt < (size_t)n) {
        ++current_m;
        if ((idx_t)current_m > (idx_t)primes_cnt || (idx_t)current_m > (idx_t)semiprimes_cnt) {
            /* Not enough precomputed primes/semiprimes; should not happen for the chosen limit. */
            break;
        }
        uint32_t p = primes[(size_t)(current_m - 1)];
        uint32_t s = semiprimes[(size_t)(current_m - 1)];
        uint32_t sum = p + s;   /* sum guaranteed <= 2*SIEVE_LIMIT < SIEVE_LIMIT */
        if (sum <= SIEVE_LIMIT && is_semiprime_uint32(sum)) {
            ensure_capacity_terms(terms_cnt + 1);
            terms[terms_cnt++] = (seq_t)sum;
        }
    }
}

/* ---------- public sequence function ------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* Undefined for 0, return 0 */
    ensure_terms(n);
    return terms[(size_t)(n - 1)];
}

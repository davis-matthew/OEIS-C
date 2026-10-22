/*
 * A393940  Numbers m such that tau(m) is a perfect number (A000396).
 * https://oeis.org/A393940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 4109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers m such that τ(m) is a perfect number (A393940). */
/* The function returns the n‑th term of the sequence (1‑based index). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ---------- global state ---------- */
static bool initialized = false;          /* flag for first call           */
static idx_t curr_limit = 0;              /* current sieve size           */
static seq_t *seq_cache = NULL;           /* array of found terms         */
static size_t seq_len   = 0;               /* number of stored terms       */
static size_t seq_cap   = 0;               /* allocated capacity of cache  */

/* ---------- perfect τ‑values to test against ---------- */
static const uint64_t perfect_tau[] = {
    6ULL, 28ULL, 496ULL, 8128ULL,
    33550336ULL, 8589869056ULL, 137438691328ULL,
    2305843008139952128ULL      /* last perfect number that fits in uint64_t  */
};
static const size_t perfect_cnt = sizeof(perfect_tau) / sizeof(perfect_tau[0]);

/* ---------- helper: test whether a divisor count is perfect ---------- */
static inline bool is_perfect_tau(uint64_t tau)
{
    for (size_t i = 0; i < perfect_cnt; ++i)
        if (tau == perfect_tau[i]) return true;
    return false;
}

/* ---------- helper: ensure seq_cache has enough capacity ---------- */
static void ensure_cache_capacity(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? (seq_cap << 1) : 128;
    if (new_cap < need) new_cap = need;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    seq_cache = new_arr;
    seq_cap   = new_cap;
}

/* ---------- build terms up to a new limit ---------- */
static void generate_terms(idx_t new_limit)
{
    /* allocate arrays for smallest prime factor (spf) and divisor count (tau) */
    uint32_t *spf = (uint32_t *)malloc((size_t)new_limit * sizeof(uint32_t));
    uint32_t *tau = (uint32_t *)malloc((size_t)new_limit * sizeof(uint32_t));
    if (!spf || !tau) exit(EXIT_FAILURE);

    /* 1. sieve for smallest prime factor  */
    for (idx_t i = 2; i < new_limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i < new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i < (uint64_t)new_limit) {
                for (idx_t j = i * i; j < new_limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    /* 2. compute divisor counts */
    tau[1] = 1;
    for (idx_t n = 2; n < new_limit; ++n) {
        uint32_t p = spf[n];
        uint32_t m = (uint32_t)n / p;
        uint32_t exp = 1;
        while (m % p == 0) { m /= p; ++exp; }
        tau[n] = tau[m] * (exp + 1);
    }

    /* 3. collect terms with perfect divisor count */
    for (idx_t n = 1; n < new_limit; ++n) {
        if (is_perfect_tau((uint64_t)tau[n])) {
            ensure_cache_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)n;
        }
    }

    free(spf);
    free(tau);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, guard against invalid use */

    /* initialize on first call */
    if (!initialized) {
        initialized = true;
        curr_limit = 1000000u;      /* start with a moderate limit        */
        generate_terms(curr_limit);
    }

    /* extend limit until we have at least n terms */
    while (seq_len < n) {
        curr_limit <<= 1;                /* double the sieve size           */
        if (curr_limit == 0) curr_limit = ~0ULL;   /* avoid overflow          */
        generate_terms(curr_limit);
    }

    return seq_cache[n - 1];
}

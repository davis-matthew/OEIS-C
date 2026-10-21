/*
 * A282251  Even integers not of the form p + prime(prime(q)) with distinct
 *          summands, where p and q are prime.
 * https://oeis.org/A282251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A282251
 * Even integers not expressible as p + prime(prime(q)) with distinct primes p, q.
 * The function a(idx_t n) returns the n-th term (1‑based) of OEIS A282251.
 *
 * Implementation details:
 *   - A sieve of Eratosthenes is used to generate the first N primes once.
 *   - For a target upper bound U, we generate all 2-(prime-index‑prime) values
 *     c = prime(prime(q)) that are ≤ U (using the prime array for indexing).
 *   - All sums p + c (with p prime ≤ U, p ≠ c) are marked.
 *   - Even numbers ≤ U not marked are collected as sequence terms.
 *   - The sequence is recomputed with a larger bound if a requested index
 *     exceeds the number of terms already generated.  The potential maximum
 *     for the 10,000th term is far below 10⁷, so a bound of 10⁷ is
 *     conservative and keeps memory and time well within limits.
 *
 * The code is strictly C11 and can also be compiled as C++17.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"

/* ---------- global prime data ------------------------------------------ */
static uint64_t *prime_pool      = NULL;   /* array of primes */
static size_t    prime_pool_cnt  = 0;     /* number of primes in prime_pool */

/* Pre‑allocate a large number of primes (≈10⁶th prime ~ 15 485 863)
 * to guarantee we can index into prime_pool for all needed indices. */
#define PRIME_SIEVE_LIMIT 15000000UL  /* 15 million */

/* ---------- sequence storage -------------------------------------------- */
static seq_t *seq_arr = NULL;  /* array of terms */
static idx_t  seq_len = 0;     /* number of terms in seq_arr */

/* ---------- helper: ensure prime pool is built --------------------------- */
static void ensure_primes(void) {
    if (prime_pool != NULL) return;
    build_primes_upto((idx_t)PRIME_SIEVE_LIMIT, &prime_pool, &prime_pool_cnt);
}

/* ---------- build sequence up to given upper bound ---------------------- */
static void build_sequence(idx_t upper) {
    ensure_primes();

    /* allocate a marker array for representations */
    char *rep = (char *)malloc((size_t)(upper + 1));
    if (!rep) { seq_len = 0; return; }
    for (size_t i = 0; i <= (size_t)upper; ++i) rep[i] = 0;

    /* ---- collect 2-(prime-index‑prime) values <= upper ---------------- */
    size_t *c_vals = NULL;      /* temporary array of seq_t */
    size_t   c_len = 0;
    size_t   c_cap = 1024;
    c_vals = (size_t *)malloc(c_cap * sizeof(size_t));
    if (!c_vals) { free(rep); return; }

    for (size_t qi = 0; qi < prime_pool_cnt; ++qi) {
        uint64_t q = prime_pool[qi];
        if (q > upper) break;            /* q itself larger than upper */

        if (q == 0 || q > prime_pool_cnt) break; /* cannot index */
        size_t idx_q = (size_t)q - 1;
        if (idx_q >= prime_pool_cnt) break;

        uint64_t r = prime_pool[idx_q];
        if (r > prime_pool_cnt) continue;   /* cannot index prime(r) */
        size_t idx_r = (size_t)r - 1;
        if (idx_r >= prime_pool_cnt) continue;

        uint64_t c = prime_pool[idx_r];
        if (c <= upper) {
            if (c_len == c_cap) {
                c_cap *= 2;
                size_t *tmp = (size_t *)realloc(c_vals, c_cap * sizeof(size_t));
                if (!tmp) { free(c_vals); free(rep); return; }
                c_vals = tmp;
            }
            c_vals[c_len++] = c;
        }
    }

    /* ---- mark all representable sums p + c ---------------------------- */
    for (size_t pi = 0; pi < prime_pool_cnt; ++pi) {
        uint64_t p = prime_pool[pi];
        if (p > upper) break;
        for (size_t ci = 0; ci < c_len; ++ci) {
            uint64_t c = c_vals[ci];
            if (p == c) continue;       /* distinct summands required */
            uint64_t s = p + c;
            if (s > (uint64_t)upper) break; /* further c will be larger */
            rep[(size_t)s] = 1;
        }
    }

    /* ---- collect even numbers not marked -------------------------------- */
    idx_t est_size = (upper / 2) + 10;           /* rough estimate */
    seq_t *new_seq = (seq_t *)malloc(est_size * sizeof(seq_t));
    if (!new_seq) { free(c_vals); free(rep); return; }

    idx_t count = 0;
    for (size_t e = 2; e <= (size_t)upper; e += 2) {
        if (!rep[e]) {
            if (count == est_size) {
                est_size *= 2;
                seq_t *tmp = (seq_t *)realloc(new_seq, est_size * sizeof(seq_t));
                if (!tmp) { free(new_seq); free(c_vals); free(rep); return; }
                new_seq = tmp;
            }
            new_seq[count++] = (seq_t)e;
        }
    }

    /* free old sequence and replace */
    free(seq_arr);
    seq_arr = new_seq;
    seq_len = count;

    free(c_vals);
    free(rep);
}

/* ---------- public API --------------------------------------------------- */
seq_t a(idx_t n) {
    /* initial bound sufficient for typical sub‑10⁶ terms */
    static bool initialized = false;
    static idx_t current_upper = 100000;   /* start with 100 k */

    if (!initialized) {
        build_sequence(current_upper);
        initialized = true;
    }

    /* enlarge upper bound until we have enough terms */
    while (n > seq_len) {
        current_upper *= 2;
        build_sequence(current_upper);
    }

    return seq_arr[n - 1];
}

/*
 * A290634  Positive integers which are never the quotient of phi(n)/tau(n).
 * https://oeis.org/A290634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer that never equals φ(m)/τ(m) for
 *            any m. The implementation precomputes φ(k) and τ(k) up to a
 *            dynamically expanded limit, records all attainable quotients,
 *            and collects the missing integers. The cached sequence is
 *            rebuilt each time the requested n exceeds the curren...
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Compute all missing quotients up to a given limit, returning an array and its length.
   The array is newly allocated; the caller is responsible for freeing it. */
static void compute_missing(idx_t limit, seq_t **out_seq, idx_t *out_len) {
    /* Allocate auxiliary arrays: phi (uint64_t to avoid overflow), tau,
       smallest prime factor (uint32_t), and prime‑exponent (uint8_t). */
    uint64_t *phi = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    uint32_t *tau = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    uint32_t *spf = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    uint8_t  *exp = (uint8_t  *)malloc((size_t)(limit + 1) * sizeof(uint8_t));
    if (!phi || !tau || !spf || !exp) {
        free(phi); free(tau); free(spf); free(exp);
        *out_seq = NULL; *out_len = 0;
        return;
    }

    /* Linear sieve for φ and τ */
    size_t  prime_cnt = 0;
    uint32_t *primes = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    if (!primes) {
        free(phi); free(tau); free(spf); free(exp);
        *out_seq = NULL; *out_len = 0;
        return;
    }

    phi[1] = 1;
    tau[1] = 1;
    spf[1] = 1;
    exp[1] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                     /* i is prime */
            spf[i] = (uint32_t)i;
            primes[prime_cnt++] = (uint32_t)i;
            phi[i] = (uint64_t)(i - 1);
            tau[i] = 2;
            exp[i] = 1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            idx_t ip = i * (idx_t)p;
            if (ip > limit) break;
            spf[ip] = p;
            if (p == spf[i]) {                 /* p divides i */
                exp[ip] = exp[i] + 1;
                phi[ip] = phi[i] * p;
                tau[ip] = tau[i] / (exp[i] + 1) * (exp[i] + 2);
                break;
            } else {                            /* p does not divide i */
                exp[ip] = 1;
                phi[ip] = phi[i] * (p - 1);
                tau[ip] = tau[i] * 2;
            }
        }
    }

    /* Mark quotients that occur */
    uint8_t *present = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!present) {
        free(phi); free(tau); free(spf); free(exp);
        free(primes);
        *out_seq = NULL; *out_len = 0;
        return;
    }

    for (idx_t i = 1; i <= limit; ++i) {
        if (phi[i] % tau[i] == 0) {
            uint32_t q = (uint32_t)(phi[i] / tau[i]); /* q <= limit for our use case */
            if (q <= limit) present[q] = 1;
        }
    }

    /* Collect missing values */
    seq_t *tmp = mem_new(seq_t, (size_t)limit);
    if (!tmp) {
        free(phi); free(tau); free(spf); free(exp);
        free(primes); free(present);
        *out_seq = NULL; *out_len = 0;
        return;
    }

    idx_t len = 0;
    for (uint32_t v = 1; v <= (uint32_t)limit; ++v) {
        if (!present[v]) {
            tmp[len++] = (seq_t)v;
        }
    }

    /* Allocate final array */
    seq_t *final = NULL;
    if (len > 0) {
        final = mem_new(seq_t, (size_t)len);
        for (idx_t k = 0; k < len; ++k) final[k] = tmp[k];
    }

    /* Clean up */
    mem_free(tmp);
    free(present);
    free(phi); free(tau); free(spf); free(exp);
    free(primes);

    *out_seq = final;
    *out_len = len;
}

/* Global cache of the sequence values */
static seq_t *cached_seq = NULL;      /* array of missing quotients */
static idx_t  cached_len = 0;         /* number of stored values   */
static idx_t  current_limit = 0;      /* sieve limit used last run */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Public function: returns the n-th term of A290634 (1‑based index). */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* below defined range */

    /* If the cache does not yet contain enough terms, rebuild with a larger limit. */
    while (cached_len < n) {
        /* Choose a reasonable sieve limit: start at 1 000 000,
           then double until we have enough terms. */
        idx_t new_limit = current_limit ? current_limit * 2 : 1000000ULL;
        seq_t *new_seq = NULL;
        idx_t new_len = 0;
        compute_missing(new_limit, &new_seq, &new_len);

        /* Replace the current cache */
        if (cached_seq) mem_free(cached_seq);
        cached_seq = new_seq;
        cached_len = new_len;
        current_limit = new_limit;
    }

    return cached_seq[n - 1];
}

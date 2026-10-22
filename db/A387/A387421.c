/*
 * A387421  Numbers k such that sigma(k) >= 2*A057521(k), where A057521(n)
 *          gives the powerful part of n, and sigma is the sum of divisors
 *          function.
 * https://oeis.org/A387421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer k for which sigma(k) >=
 *            2*A057521(k). sigma(k) is the sum of the divisors of k.
 *            A057521(k) is the powerful part of k, i.e. the product of all
 *            prime powers pᵉ with e ≥ 2 in the factorization of k
 * Range:     u8 n <= 17, u16 n <= 1432, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* Dynamic array of terms discovered so far. */
static seq_t *terms = NULL;
static size_t terms_size = 0;
static size_t terms_cap  = 0;
static uint64_t last_checked = 0;   /* last integer examined (inclusive) */

/* Ensure the internal array can hold at least *needed* elements. */
static void ensure_terms_capacity(size_t needed) {
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* Return true if k satisfies sigma(k) >= 2*A057521(k). */
static bool qualifies(uint64_t k) {
    /* Factorize k: primes[i]^exps[i] (cnt distinct primes). */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);

    seq_t sigma   = 1;
    seq_t powerful = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* σ(pᵉ) = (p^(e+1) – 1) / (p – 1) */
        seq_t num = pow_uint(p, e + 1);
        num -= (seq_t)1;
        seq_t den = (seq_t)(p - 1);
        seq_t sigma_i = num / den;
        sigma *= sigma_i;

        /* Multiply by pᵉ if e ≥ 2 – this contributes to A057521(k). */
        if (e >= 2) {
            powerful *= pow_uint(p, e);
        }
    }

    /* Compare σ(k) with 2 * powerful part */
    return sigma >= powerful * (seq_t)2;
}

/* Generate terms until the array contains at least *n* elements. */
static void generate_up_to(size_t n) {
    if (terms_size >= n) return;
    ensure_terms_capacity(n);

    uint64_t k;
    for (k = last_checked + 1; ; ++k) {
        if (qualifies(k)) {
            terms[terms_size++] = (seq_t)k;
            if (terms_size >= n) break;
        }
    }
    /* The loop finishes when *k* has been processed and the
       required number of terms have been collected. */
    last_checked = k;
}

/* Public entry point required by the framework. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined below offset 1 */
    generate_up_to((size_t)n);
    return terms[(size_t)n - 1];
}

/*
 * A324649  Numbers k such that A318458(k) (bitwise-AND of k and sigma(k)-k)
 *          is equal to k.
 * https://oeis.org/A324649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 729, u32 n <= 9830, u64 n <= 9830,
 *            u128 n <= 9830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A324649: numbers k such that A318458(k) (= k & (σ(k)-k))
 * is equal to k.  We generate the nth element of the sequence
 * (a(1)=6, a(2)=20, …) on demand by caching both the divisor sums
 * σ(k) and the found terms.  The implementation works for all
 * supported build types (fixed‑width or arbitrary‑precision seq_t).
 * It uses only integer arithmetic and a simple sieve.
 */

#include <stdlib.h>

static idx_t sigma_limit = 0;       /* current maximum k with σ data */
static seq_t *sigma_arr = NULL;     /* σ(k) array, 1‑based, size = sigma_limit+1 */

static size_t terms_cap = 0;        /* capacity of cached terms array */
static seq_t *terms = NULL;        /* found terms, zero‑based index */
static size_t terms_cnt = 0;        /* how many terms are stored */

/* last integer examined when searching for new terms */
static idx_t last_checked = 0;

/* Recompute σ(k) for all 1 ≤ k ≤ new_limit using a classic divisor sieve.
 * Allocates or resizes sigma_arr as necessary. */
static void recompute_sigma(idx_t new_limit) {
    seq_t *new_sigma = (seq_t*)realloc(sigma_arr, (size_t)(new_limit + 1) * sizeof(seq_t));
    if (!new_sigma) abort();
    sigma_arr = new_sigma;
    for (idx_t i = 0; i <= new_limit; ++i) sigma_arr[i] = 0;

    for (idx_t i = 1; i <= new_limit; ++i) {
        seq_t v = (seq_t)i;
        for (idx_t j = i; j <= new_limit; j += i)
            sigma_arr[j] += v;
    }
    sigma_limit = new_limit;
}

/* Ensure that σ(k) is available for all k up to required_limit. */
static void ensure_sigma(idx_t required_limit) {
    if (required_limit <= sigma_limit) return;
    idx_t new_limit = sigma_limit ? sigma_limit * 2 : 256;
    while (new_limit < required_limit) new_limit *= 2;
    recompute_sigma(new_limit);
}

/* Ensure that the terms array has space for at least needed items. */
static void ensure_terms_cap(size_t needed) {
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) abort();
    terms = new_terms;
    terms_cap = new_cap;
}

/* Append a new term to the cache. */
static void add_term(seq_t val) {
    ensure_terms_cap(terms_cnt + 1);
    terms[terms_cnt++] = val;
}

/* Return the n-th term of sequence A324649 (1‑based index). */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* behaviour below offset is unspecified */

    if (n <= terms_cnt) return terms[n - 1];

    /* search for further terms until we have at least n of them */
    for (idx_t k = last_checked + 1; ; ++k) {
        ensure_sigma(k);
        seq_t sig = sigma_arr[k];
        seq_t diff = sig - (seq_t)k;      /* σ(k) – k = sum of proper divisors */
        if (((k & diff) == k)) {
            add_term((seq_t)k);
            last_checked = k;
            if (terms_cnt >= (size_t)n) break;
        }
    }
    return terms[n - 1];
}

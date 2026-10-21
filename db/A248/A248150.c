/*
 * A248150  Numbers whose sum of divisors (A000203) is divisible by 4.
 * https://oeis.org/A248150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 169, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose sum of divisors σ(n) is divisible by 4.
   Computed by incrementally sieving σ(n) up to a dynamic limit until
   the requested number of terms is available. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Static state for caching results */
static idx_t   max_processed = 0;   /* largest n that has been sieved */
static seq_t*  sigma_cache   = NULL;/* array of σ values, 1‑based */
static size_t  sigma_cap     = 0;    /* allocated size of sigma_cache */

static seq_t*  seq_terms     = NULL;/* terms of the sequence, 0‑based */
static size_t  terms_len     = 0;    /* number of terms stored */

/* Helper: recompute σ up to the given limit and build the terms array. */
static void recompute_terms(idx_t limit)
{
    /* Allocate and zero the σ array */
    seq_t* new_sigma = (seq_t*)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (!new_sigma) {
        /* In case of allocation failure we simply abort; the test harness
           guarantees sufficient memory for the limits we use. */
        exit(1);
    }

    /* Compute σ(i) for all i <= limit */
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i) {
            new_sigma[j] += (seq_t)i;
        }
    }

    /* Count how many numbers satisfy σ(n) % 4 == 0 */
    size_t count = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if ((new_sigma[i] & (seq_t)3) == 0)   /* equivalent to %4 == 0 */
            ++count;
    }

    /* Allocate terms array */
    seq_t* new_terms = (seq_t*)malloc(count * sizeof(seq_t));
    if (!new_terms) {
        free(new_sigma);
        exit(1);
    }

    /* Fill terms array */
    size_t pos = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if ((new_sigma[i] & (seq_t)3) == 0)
            new_terms[pos++] = (seq_t)i;
    }

    /* Replace old caches */
    free(seq_terms);
    free(sigma_cache);
    seq_terms   = new_terms;
    terms_len   = count;
    sigma_cache = new_sigma;
    sigma_cap   = (size_t)(limit + 1);
    max_processed = limit;
}

/* Ensure at least 'required' terms are available. */
static void ensure_terms(idx_t required)
{
    if (required <= terms_len)
        return;                        /* already have enough */

    /* Start with a limit that is at least twice the requested index,
       but not smaller than a small base value. */
    idx_t limit = required * 2;
    if (limit < 1000)
        limit = 1000;

    /* Increase the limit until we have enough terms. */
    while (true) {
        recompute_terms(limit);

        if (terms_len >= required)
            break;                    /* satisfied */

        /* Not enough, double the limit and try again. */
        if (limit > ((idx_t)UINT64_MAX >> 1))
            limit = UINT64_MAX;       /* avoid overflow */
        else
            limit <<= 1;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* the sequence is 1‑based; this is a sentinel. */

    ensure_terms(n);
    return seq_terms[n - 1];
}

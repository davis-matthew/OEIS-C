/*
 * A294574  Numbers having exactly one representation as sum of squares of
 *          primes.
 * https://oeis.org/A294574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This program computes the OEIS sequence A294574:
   Numbers having exactly one representation as a sum
   of squares of primes (order does not matter, primes may repeat). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"

/* Integer square root (floor) for idx_t (uint64_t).  */
static inline idx_t idx_sqrt(idx_t n) {
    idx_t lo = 0, hi = n >> 1U;
    if (n <= 3U) return n;
    hi = hi + 1U;                      /* ensure hi^2 > n */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1U) >> 1U;
        if (mid > 0 && mid > n / mid) /* avoid overflow, mid*mid > n */
            hi = mid - 1U;
        else
            lo = mid;
    }
    return lo;
}

/* Global state holding the DP array (capped at 2) and the list of terms. */
static unsigned char *dp_array  = NULL;   /* dp values capped at 2 */
static size_t         dp_len    = 0;       /* dp_array size (limit+1) */
static seq_t *terms  = NULL;              /* list of terms (value of the sequence) */
static size_t  terms_len = 0;              /* number of terms currently stored */
static idx_t   search_limit = 0;           /* maximum n value that has been computed */

/* Recompute dp_array and the term list up to the given inclusive limit. */
static void recompute_up_to(idx_t limit) {
    /* free previous structures */
    free(dp_array);
    dp_array = NULL;
    dp_len   = 0;
    free(terms);
    terms    = NULL;
    terms_len = 0;
    search_limit = 0;

    /* 1. Get all prime squares <= limit.  */
    idx_t sqrt_limit = idx_sqrt(limit);
    size_t prime_cnt = 0;
    uint64_t *prime64 = NULL;
    build_primes_upto(sqrt_limit, &prime64, &prime_cnt);

    /* 2. Allocate DP array. */
    dp_len = (size_t)limit + 1U;
    dp_array = (unsigned char*)calloc(dp_len, sizeof(unsigned char));
    if (!dp_array) {
        free(prime64);
        return;
    }
    dp_array[0] = 1;  /* empty sum base case */

    /* 3. DP: coin‑change counting with prime squares (unordered). */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = (idx_t)prime64[pi];
        idx_t p2 = (idx_t)p * (idx_t)p;     /* square of the prime */

        for (idx_t j = p2; j <= limit; ++j) {
            unsigned char sum = dp_array[(size_t)j] + dp_array[(size_t)(j - p2)];
            dp_array[(size_t)j] = (sum > 2U ? 2U : sum);
        }
    }
    free(prime64);

    /* 4. Build term list: numbers with exactly one representation. */
    size_t count = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (dp_array[(size_t)i] == 1U) ++count;
    }

    terms = (seq_t *)malloc(count * sizeof(seq_t));
    if (!terms) {
        free(dp_array);
        dp_array = NULL;
        dp_len   = 0;
        return;
    }

    size_t idx = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (dp_array[(size_t)i] == 1U)
            terms[idx++] = (seq_t)i;
    }
    terms_len   = count;
    search_limit = limit;
}

/* Ensure that at least `n` terms have been computed.  Double the limit
   whenever a larger value is required.  This routine is O(n) overall. */
static void ensure_terms(idx_t n) {
    if (terms_len >= (size_t)n) return;   /* already have enough terms */

    idx_t limit = search_limit > 0 ? search_limit << 1 : 100U;
    while (terms_len < (size_t)n) {
        recompute_up_to(limit);
        if (terms_len >= (size_t)n) break;
        limit <<= 1;          /* double the search limit if needed */
    }
}

/* Public sequence function.  Indexing starts at 1.  For n == 0,
   the function returns 0 as a harmless default. */
seq_t a(idx_t n) {
    if (n == 0) return 0;

    ensure_terms(n);

    if (n > terms_len) return terms[terms_len - 1];
    return terms[(size_t)(n - 1)];
}

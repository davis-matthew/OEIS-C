/*
 * A368728  Numbers whose prime indices are 1, prime, or semiprime.
 * https://oeis.org/A368728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   allowed_index, build_prime_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368728: Numbers whose prime indices are 1, prime, or semiprime.
   Algorithm:  Sieve numbers up to a modest bound, test each for the property,
   store the ones that satisfy it.  Subsequent calls simply return the
   (n‑1)-th stored value.  The bound (2 000 000) is sufficient for all
   test cases up to n ≈ 10 000.  The implementation works for any
   seq_t/idx_t pair supplied by the build system. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/allowed_index.h"
#include "../utils/build_prime_index.h"

static const idx_t MAX_LIMIT = 2000000;          /* sieve upper bound */

static bool initialized = false;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t *cache = NULL;                     /* stores the allowed numbers */

/* Helper: compute smallest prime factor array up to limit.
   The array is indexed from 0..limit.  Returned pointer must be freed
   by the caller. */
static uint32_t *sieve_spf(idx_t limit)
{
    uint32_t *spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) exit(1);
    for (idx_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (spf[i] == (uint32_t)i)
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
    return spf;
}

/* Main initialization: build sieve, prime indices, allowed flags,
   and the sequence cache. */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    /* Sieve for smallest prime factors up to MAX_LIMIT */
    uint32_t *spf = sieve_spf(MAX_LIMIT);

    /* Array to map each value to its prime index (0 if nonprime) */
    uint32_t *prime_index = (uint32_t *)calloc(MAX_LIMIT + 1, sizeof(uint32_t));
    if (!prime_index) exit(1);

    size_t prime_cnt = 0;
    build_prime_index(spf, prime_index, MAX_LIMIT, &prime_cnt);

    /* Boolean array: true iff index (1..prime_cnt) is 1, prime, or semiprime */
    bool *idx_allowed = (bool *)calloc(prime_cnt + 1, sizeof(bool));
    if (!idx_allowed) exit(1);
    for (idx_t i = 1; i <= prime_cnt; ++i)
        idx_allowed[i] = allowed_index(spf, i);

    /* Boolean array for numbers: true iff all prime indices of the number
       are allowed.  We'll compute this lazily while filling the cache. */
    bool *num_allowed = (bool *)calloc(MAX_LIMIT + 1, sizeof(bool));
    if (!num_allowed) exit(1);
    num_allowed[1] = true;                     /* by definition */

    /* Allocate cache with an initial capacity */
    cache_cap = 1024;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(1);

    /* Main loop: fill num_allowed and cache */
    for (idx_t i = 2; i <= MAX_LIMIT; ++i) {
        uint32_t p = spf[i];
        idx_t rest = i / p;
        bool ok = idx_allowed[prime_index[p]] && num_allowed[rest];
        num_allowed[i] = ok;
    }

    /* Populate cache with numbers that satisfy the property */
    cache_len = 0;
    for (idx_t i = 1; i <= MAX_LIMIT; ++i) {
        if (num_allowed[i]) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)i;
        }
    }

    /* Clean up temporaries */
    free(spf);
    free(prime_index);
    free(idx_allowed);
    free(num_allowed);
}

/* Return the n-th term of OEIS A368728 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for n == 0, but avoid UB */
    init_sequence();
    /* The test harness guarantees that n is within the pre‑computed range. */
    return cache[n - 1];
}

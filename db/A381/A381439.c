/*
 * A381439  Numbers whose exponent of 2 in their canonical prime factorization
 *          is not larger than all the other exponents.
 * https://oeis.org/A381439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 197, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes A381439: numbers whose exponent of 2 in their prime factorization
 * is not larger than any other exponent.
 * The function builds the sequence on the fly and caches results so the
 * same numbers are not recomputed for subsequent calls.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Count the number of terms remembered so far */
static size_t cache_size = 0;
/* Current capacity of the cache */
static size_t cache_capacity = 0;
/* Pointer to the cache of sequence values */
static seq_t *cache = NULL;
/* Next integer to test in the sequence generation process */
static uint64_t next_candidate = 3;

static bool ok_uint64(uint64_t n) {
    /* Count exponent of 2 */
    uint64_t e2 = 0;
    uint64_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        ++e2;
        tmp >>= 1;
    }
    /* If no odd part, the number is a pure power of 2 → exclude */
    if (tmp == 1)
        return false;

    /* Factor the number to find the largest exponent among odd primes */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);

    uint64_t max_other = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] != 2 && ex[i] > max_other)
            max_other = ex[i];
    }
    return e2 <= max_other;
}

seq_t a(idx_t n) {
    /* The sequence is 1‑based; n >= 1 for all meaningful calls. */
    /* Ensure the cache contains at least n terms. */
    while (cache_size < n) {
        uint64_t m = next_candidate++;
        if (ok_uint64(m)) {
            /* Expand cache if necessary */
            if (cache_size == cache_capacity) {
                size_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache)
                    return 0; /* Allocation failure */
                cache = new_cache;
                cache_capacity = new_cap;
            }
            cache[cache_size++] = (seq_t)m;
        }
    }
    return cache[n - 1];
}

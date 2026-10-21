/*
 * A264845  Centered 20-gonal (or icosagonal) primes.
 * https://oeis.org/A264845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   isPrimeGeneric, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Centered 20‑gonal primes.
   Each term is the n‑th prime of the form 10*k^2 - 10*k + 1. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Static state used for memoisation. */
static seq_t *cache       = NULL; /* Cached prime values. */
static size_t cache_cap   = 0;    /* Allocated capacity of cache. */
static size_t cache_len   = 0;    /* Number of primes stored. */
static idx_t   next_k      = 1;   /* Next integer k to test. */

/* Return the n‑th term of the sequence, with offset 1. */
seq_t a(idx_t n)
{
    /* Determine the maximum value that can be represented by seq_t. */
    unsigned __int128 max_val;
    switch (sizeof(seq_t)) {
        case 1:  max_val = UINT8_MAX;   break;
        case 2:  max_val = UINT16_MAX;  break;
        case 4:  max_val = UINT32_MAX;  break;
        case 8:  max_val = UINT64_MAX;  break;
        case 16: max_val = (unsigned __int128)(-1); break; /* all bits 1 */
        default: max_val = (unsigned __int128)(-1); break;
    }

    /* Generate primes until we have at least n of them or we ran out of range. */
    while (cache_len < n) {
        unsigned __int128 kk      = (unsigned __int128)next_k;
        unsigned __int128 val128  = 10 * kk * kk - 10 * kk + 1;

        /* If val128 cannot be represented in seq_t, we cannot store it,
           so stop generating further primes. */
        if (val128 > max_val) {
            break;
        }

        seq_t val = (seq_t)val128;
        if (isPrimeGeneric(val)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        ++next_k;
    }

    /* If we did not manage to find enough primes (overflowed seq_t range),
       return 0.  The specification only requires correctness when the
       result fits into seq_t. */
    if (cache_len < n) {
        return 0;
    }
    return cache[n - 1];
}

/*
 * A263722  Integers k > 0 such that k^2 + p^2 is composite for all primes p.
 * https://oeis.org/A263722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A263722: integers k>0 such that k^2 + p^2 is composite for all primes p.  
   Equivalent to odd k with k^2 + 4 composite.  
   Computed by testing odd candidates and storing those whose k^2+4 is composite. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/isPrimeGeneric.h"

static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static idx_t current_k = 1;   /* last k processed; starts below 3 */

/* enlarge the cache when needed */
static void seq_grow(void) {
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
#if defined(__cplusplus)
    seq_cache = reinterpret_cast<seq_t*>(realloc(seq_cache, new_cap * sizeof(seq_t)));
#else
    seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
#endif
    seq_cap = new_cap;
}

seq_t a(idx_t n) {
    while (seq_len < (size_t)n) {
        /* advance to the next odd integer starting from 3 */
        if (current_k < 3)
            current_k = 3;
        else
            current_k += 2;

        seq_t val = (seq_t)current_k * (seq_t)current_k + (seq_t)4;

        if (!isPrimeGeneric(val)) {
            if (seq_len == seq_cap)
                seq_grow();
            seq_cache[seq_len++] = (seq_t)current_k;
        }
    }
    return seq_cache[(size_t)n - 1];
}

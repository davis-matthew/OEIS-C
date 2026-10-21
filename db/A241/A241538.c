/*
 * A241538  Squares s such that s + 1234567890 is prime.
 * https://oeis.org/A241538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = smallest square s such that s + 1234567890 is prime (OEIS
 *            A241538)
 * Range:     u8 n <= 1, u16 none, u32 n <= 2327, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is ordered by increasing square value. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cache of the discovered square values. */
static seq_t *cache      = NULL; /* array of squares */
static size_t cache_cap  = 0;    /* allocated capacity */
static size_t cache_len  = 0;    /* number of squares stored */
static idx_t   next_m    = 1;    /* next integer m to test */

/* Constant added to each square. */
static const uint64_t BASE_CONST_U64 = 1234567890ULL;
static const seq_t BASE_CONST = (seq_t)BASE_CONST_U64;

/* Return the n‑th term of the sequence (1‑based index).  
   If n <= 0, return 0 as a sentinel value. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined below offset */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Compute m^2 as a 64‑bit value and cast to seq_t. */
        uint64_t m_sq64 = (uint64_t)next_m * (uint64_t)next_m;
        seq_t m_sq = (seq_t)m_sq64;            /* square as seq_t */

        /* Candidate to test for primality: m^2 + 1234567890. */
        seq_t candidate = m_sq + BASE_CONST;   /* addition wraps if overflow */

        /* Check primality of the candidate. */
        if (isPrimeGeneric(candidate)) {
            /* Ensure there is room in the cache. */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (tmp == NULL) /* allocation failure – abort gracefully */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m_sq; /* store the square */
        }

        ++next_m; /* advance to the next integer m */
    }

    return cache[n - 1];
}

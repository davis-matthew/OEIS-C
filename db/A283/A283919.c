/*
 * A283919  The smallest square referenced in A013929 (Numbers that are not
 *          squarefree).
 * https://oeis.org/A283919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   init_spf, smallest_square_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A283919: a(n) = the smallest square that divides the nth
 * non‑squarefree integer (i.e., the smallest p² such that p²│n).
 * The algorithm generates non‑squarefree integers in order, uses a
 * smallest‑prime‑factor sieve for factorisation, and memoises the
 * resulting sequence values.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/init_spf.h"
#include "../utils/smallest_square_factor.h"

/* ------------------------------------------------------------ */
/*                 Static helper definitions                  */
/* ------------------------------------------------------------ */

/* Upper bound for the sieve (covers >10000 terms with safety margin) */
#define SPF_UPPER_LIMIT 200000

/* ------------------------------------------------------------ */
/*                 Public sequence function                    */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Static state */
    static bool initialized = false;
    static idx_t *spf = NULL;        /* smallest prime factor table   */
    static seq_t *cache = NULL;      /* cached sequence values        */
    static size_t cache_len = 0;     /* number of terms stored        */
    static size_t cache_cap = 0;     /* capacity of cache array       */
    static idx_t next_candidate = 4; /* next integer to test          */

    /* One‑time initialisation */
    if (!initialized) {
        /* Allocate and initialise SPF table */
        spf = (idx_t *)calloc(SPF_UPPER_LIMIT + 1, sizeof(idx_t));
        if (!spf) return 0;           /* allocation failure – unlikely  */
        init_spf(spf, SPF_UPPER_LIMIT);

        /* Allocate cache for sequence terms */
        cache_cap = 2000;            /* generous initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) {
            free(spf);
            return 0;                /* allocation failure            */
        }

        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* If we exceed the sieve limit, we would need to extend it.
           For our target (n≈10000) SPF_UPPER_LIMIT is sufficient. */
        if (next_candidate > SPF_UPPER_LIMIT) {
            /* In practice this does not happen for the intended limits.
               Returning 0 indicates an error conditions. */
            return 0;
        }

        seq_t sq = smallest_square_factor(next_candidate, spf);
        if (sq != 0) {                     /* non‑squarefree number */
            /* Grow cache if needed */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    free(spf);
                    return 0;            /* allocation failure      */
                }
                cache = tmp;
            }
            cache[cache_len++] = sq;
        }
        ++next_candidate;
    }

    /* Return the requested term (1‑based index). */
    if (n == 0 || n > cache_len) {
        return 0;                         /* undefined for out‑of‑range */
    }
    return cache[n - 1];
}

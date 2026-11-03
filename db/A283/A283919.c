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
 * Validated: standard, pass, 98 terms through n = 9842 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* Ensure that the SPF array is initialised and contains values up to
 * SPF_UPPER_LIMIT. This function is called only once. */
static void init_spf(idx_t *spf_arr, idx_t limit)
{
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf_arr[i] == 0) {
            spf_arr[i] = i;
            if (i <= limit / i) {
                idx_t ii = i * i;
                for (idx_t j = ii; j <= limit; j += i) {
                    if (spf_arr[j] == 0) {
                        spf_arr[j] = i;
                    }
                }
            }
        }
    }
}

/* Compute the smallest square factor of a non‑squarefree number.
 * Returns 0 if the number is squarefree (should not happen for our
 * callers). */
static seq_t smallest_square_factor(idx_t x, idx_t *spf)
{
    while (x > 1) {
        idx_t p = spf[x];
        idx_t exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }
        if (exp >= 2) {
            seq_t sq = (seq_t)p * (seq_t)p;
            return sq;
        }
    }
    return 0;                /* squarefree */
}

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
        cache = mem_new(seq_t, cache_cap);
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
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp) {
                    mem_free(cache);
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

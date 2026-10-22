/*
 * A372069  Value of p^2 + (p-1)^2/2 as p runs through A001359 (the lesser of
 *          twin primes).
 * https://oeis.org/A372069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n^2 + (p_n-1)^2/2 where p_n is the nth twin‐prime lower
 *            member (p, p+2)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Cache for the computed terms. */
static seq_t *cache = NULL;      /* cache[1] is a(1) */
static idx_t cache_size = 0;     /* number of terms cached (≥0) */
static size_t cache_cap = 0;     /* allocated capacity */

/* Last odd candidate checked for primality.  Starts at 3. */
static idx_t last_checked = 3;

/* Return the nth term of A372069.  n is 1‑based.  Behavior for n<1 is
   undefined; we return 0 for n==0 to keep the function safe. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Initialise static structures on first use. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)calloc(cache_cap + 1, sizeof(seq_t));

        /* Ensure the array is usable up to cache_cap+1. */
        cache[0] = (seq_t)0;   /* unused slot for 0th index */
    }

    /* Extend cache until it contains the nth term. */
    while (cache_size < n) {
        idx_t p = last_checked;
        last_checked += 2;               /* next odd candidate */

        if (!is_prime(p))
            continue;                    /* skip non‑primes */

        /* Check twin condition: p+2 must also be prime. */
        if (!is_prime(p + 2U))
            continue;                    /* not a twin pair */

        /* Compute the term.  All arithmetic is done in seq_t. */
        seq_t pc = (seq_t)p;                     /* lower twin prime */
        seq_t val = pc * pc + ((pc - (seq_t)1) * (pc - (seq_t)1)) / (seq_t)2;

        /* Grow cache if necessary. */
        if (cache_size + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_buf = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
            if (!new_buf) {
                /* Allocation failure – abort gracefully. */
                /* In a production build we might exit or handle the error. */
                return (seq_t)0;
            }
            cache = new_buf;
            cache_cap = new_cap;
        }

        cache[++cache_size] = val;
    }

    return cache[n];
}

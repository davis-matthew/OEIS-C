/*
 * A335277  First index of strictly increasing prime quartets.
 * https://oeis.org/A335277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A335277 – first index of strictly increasing prime quartets. */
/* Sequence a(n) is the n‑th index i (1‑based) such that for primes
   p(i),p(i+1),p(i+2),p(i+3) the gaps satisfy  p(i+1)-p(i) < p(i+2)-p(i+1)
   < p(i+3)-p(i+2).  The function computes a(n) by generating primes
   on demand and scanning gaps. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes.h"

static idx_t *prime_list = NULL;   /* array of primes (1‑based indices in array) */
static size_t prime_count = 0;     /* number of primes in prime_list */
static idx_t sieve_limit = 0;      /* current upper bound of the sieve */

static idx_t *indices_list = NULL; /* list of found quartet start indices (1‑based) */
static size_t indices_used = 0;
static size_t indices_cap  = 0;

static idx_t next_start = 0;       /* next index to test (0‑based into prime_list) */

/* Append a new index to indices_list, growing the array if necessary. */
static void append_index(idx_t val)
{
    if (indices_used == indices_cap) {
        size_t new_cap = indices_cap == 0 ? 16 : indices_cap * 2;
        idx_t *tmp = (idx_t *)realloc(indices_list, new_cap * sizeof(idx_t));
        if (!tmp) exit(1); /* allocation failure */
        indices_list = tmp;
        indices_cap  = new_cap;
    }
    indices_list[indices_used++] = val;
}

/* Reallocate the prime list so that it contains all primes up to at least new_limit. */
static void ensure_primes(idx_t new_limit)
{
    if (new_limit <= sieve_limit) return;

    if (prime_list) free(prime_list);
    build_primes(new_limit, &prime_list, &prime_count);
    sieve_limit = new_limit;
}

/* Ensure that at least n_indices quartet start indices have been found. */
static void ensure_n_indices(idx_t n_indices)
{
    while (indices_used < n_indices) {
        /* If we don't yet have enough primes to examine the next quartet, extend. */
        if (next_start + 4 > prime_count) {
            /* Pick a new, larger bound.  Start with a reasonable default,
               but always double until we have enough primes. */
            idx_t lim = sieve_limit > 0 ? sieve_limit * 2 : 2000000ULL;
            /* Keep doubling until the sieve_limit covers next_start+4 */
            while (lim <= next_start + 4) lim *= 2;
            ensure_primes(lim);
            continue;
        }

        /* Compute the three consecutive gaps. */
        idx_t g1 = prime_list[next_start + 1] - prime_list[next_start];
        idx_t g2 = prime_list[next_start + 2] - prime_list[next_start + 1];
        idx_t g3 = prime_list[next_start + 3] - prime_list[next_start + 2];

        if (g1 < g2 && g2 < g3) {
            /* Store the 1‑based index of the first prime in this quartet. */
            append_index(next_start + 1);
        }
        ++next_start;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* only defined for n ≥ 1, but safe guard */
    ensure_n_indices(n);
    return (seq_t)indices_list[n - 1];
}

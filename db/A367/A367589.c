/*
 * A367589  Numbers with exactly two distinct prime factors, both appearing
 *          with different exponents.
 * https://oeis.org/A367589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367589: numbers with exactly two distinct prime factors,
   both appearing with different exponents.
   The function builds a cache of all such numbers up to a limit that
   guarantees at least n entries, then returns the nth (1‑based) term.   */

#include <stdlib.h>
#include <stdint.h>

/* Utility prototypes provided by the build environment.  */
#include "../utils/build_primes_upto.h"

/* Forward declaration of the helper that generates the numbers. */
static void generate_numbers(uint64_t limit, seq_t **out, size_t *out_count);

/* Comparator for qsort. */
static int seq_compare(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Cached sequence data. */
static seq_t *cache       = NULL;   /* array of sequence values, sorted */
static size_t cache_size  = 0;      /* number of values stored          */
static uint64_t cache_lim = 0;      /* largest number previously generated */

/* Public entry point.  */
seq_t a(idx_t n)
{
    if (n == 0) {                     /* the sequence is 1‑based only */
        return (seq_t)0;
    }

    /* Increase the cache until we have at least n terms. */
    while (cache_size < (size_t)n) {
        /* Pick a new limit.  Double the previous limit or start at 200_000. */
        uint64_t new_lim = cache_lim ? cache_lim * 2ULL : 200000ULL;

        seq_t *new_vals  = NULL;
        size_t new_count = 0;

        generate_numbers(new_lim, &new_vals, &new_count);

        /* Replace the old cache with the new one. */
        free(cache);
        cache       = new_vals;
        cache_size  = new_count;
        cache_lim   = new_lim;
    }

    return cache[n - 1];
}

/* Generate all integers <= limit that have exactly two distinct prime
   factors with different exponents.  The array is sorted and returned
   in *out; *out_count gives its length.  Caller must free *out. */
static void generate_numbers(uint64_t limit, seq_t **out, size_t *out_count)
{
    /* 1. Obtain the list of primes up to 'limit'. */
    uint64_t *primes = NULL;
    size_t    pcnt   = 0;
    build_primes_upto((idx_t)limit, &primes, &pcnt);
    if (pcnt == 0) {
        *out = NULL;
        *out_count = 0;
        return;
    }

    /* 2. Temporary buffer for results. */
    size_t cap = 256;
    seq_t *vals = (seq_t *)malloc(cap * sizeof(seq_t));
    size_t len  = 0;

    /* 3. Enumerate admissible pairs (p,q). */
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];

        /* powers of p */
        uint64_t p_pow = p;
        int a = 1;
        while (p_pow <= limit) {
            for (size_t j = i + 1; j < pcnt; ++j) {
                uint64_t q = primes[j];

                uint64_t q_pow = q;
                int b = 1;
                /* powers of q, ensure product <= limit */
                while (p_pow * q_pow <= limit) {
                    if (a != b) {
                        /* Append product */
                        if (len == cap) {
                            cap <<= 1;
                            vals = (seq_t *)realloc(vals, cap * sizeof(seq_t));
                        }
                        vals[len++] = (seq_t)(p_pow * q_pow);
                    }
                    /* Next power of q */
                    if (q_pow > limit / q) break;
                    q_pow *= q;
                    ++b;
                }
            }
            /* Next power of p */
            if (p_pow > limit / p) break;
            p_pow *= p;
            ++a;
        }
    }

    free(primes);

    /* 4. Sort the collected numbers. */
    qsort(vals, len, sizeof(seq_t), seq_compare);

    *out = vals;
    *out_count = len;
}

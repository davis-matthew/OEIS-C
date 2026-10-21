/*
 * A249044  Composite self-numbers.
 * https://oeis.org/A249044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 5824, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, sumdigits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the composite self‑numbers (OEIS A249044)
   by sieving the generator g(m) = m + sumdigits(m) and selecting
   composite non‑generated numbers.
   a(1) = 9, a(2) = 20, a(3) = 42, … */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/sumdigits.h"

/*-----------------  helper data structures  -----------------*/

static unsigned char *generated = NULL;      /* 1 iff a number appears as g(m) */
static size_t generated_cap = 0;
static idx_t processed_m = 0;                /* largest m for which g(m) was computed */
static idx_t last_checked = 1;               /* last integer examined */

static void ensure_generated(size_t limit)
{
    if (generated_cap <= limit) {
        size_t new_cap = generated_cap ? generated_cap * 2 : 8;
        while (new_cap <= limit) new_cap <<= 1;
        unsigned char *tmp = (unsigned char *)realloc((void *)generated,
                                                       new_cap * sizeof(unsigned char));
        if (!tmp) abort();
        memset(tmp + generated_cap, 0,
               (new_cap - generated_cap) * sizeof(unsigned char));
        generated = tmp;
        generated_cap = new_cap;
    }
}

/*-----------------  cache of computed sequence  -----------------*/

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc((void *)cache,
                                      new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/*-----------------  main sequence routine  -----------------*/

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety for callers using 0 as illegal index */

    while (cache_len < (size_t)n) {
        /* next integer to test as a possible term */
        idx_t candidate = last_checked + 1;
        last_checked = candidate;

        /* generate g(m) for all m < candidate that have not yet been processed */
        while (processed_m < candidate) {
            idx_t m = processed_m + 1;
            idx_t g = m + sumdigits(m);
            ensure_generated((size_t)g);
            generated[(size_t)g] = 1;
            processed_m = m;
        }

        /* make sure the table can answer generated[candidate] */
        ensure_generated((size_t)candidate);

        /* candidate is a composite self‑number iff it is not generated
           and not a prime number */
        if (!generated[(size_t)candidate] && candidate > 1 &&
            !is_prime(candidate)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)candidate;
        }
    }
    return cache[(size_t)(n - 1)];
}

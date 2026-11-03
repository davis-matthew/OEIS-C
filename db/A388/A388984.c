/*
 * A388984  Even numbers, not powers of primes, that are of the form p * m^2,
 *          where p is prime and m by necessity > 1.
 * https://oeis.org/A388984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th even integer of the form p * m² where p is
 *            prime, m > 1, and the integer has at least two distinct prime
 *            factors. Computed by incremental search
 * Range:     u8 n <= 33, u16 n <= 3867, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9735 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* Helper: test whether a given even integer N satisfies the
   description of A388984. */
static bool is_even_prime_square_form(seq_t N)
{
    /* N must be even */
    if ((N & 1) != 0)
        return false;

    idx_t n = (idx_t)N;          /* value fits in idx_t for the ranges used. */
    idx_t count_distinct = 0;
    idx_t count_odd      = 0;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            ++count_distinct;
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if ((exp & 1) != 0)
                ++count_odd;
        }
    }
    if (n > 1) {                 /* remaining prime factor */
        ++count_distinct;
        ++count_odd;              /* exponent is 1 => odd */
    }

    return (count_odd == 1 && count_distinct >= 2);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static seq_t next_candidate = 12;  /* first even candidate */

    if (n == 0) /* undefined for offset 1, return 0 */
        return 0;

    /* Ensure cache large enough to hold n terms */
    if (n > cache_size) {
        size_t needed = (size_t)n;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < needed)
                new_cap = needed;
            seq_t *new_cache = mem_new(seq_t, new_cap);
            if (!new_cache)
                abort();
            if (cache) {
                mem_copy(new_cache, cache, cache_size * sizeof(seq_t));
                mem_free(cache);
            }
            cache = new_cache;
            cache_cap = new_cap;
        }
        /* Generate terms up to index n */
        while (cache_size < n) {
            while (!is_even_prime_square_form(next_candidate))
                next_candidate += 2;
            cache[cache_size++] = next_candidate;
            next_candidate += 2; /* advance to next even number */
        }
    }
    return cache[n - 1];
}

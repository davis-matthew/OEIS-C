/*
 * A392051  Nonnegative integers such that the harmonic mean of the digits is
 *          an integer.
 * https://oeis.org/A392051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   harmonic_integer
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392051: a(n) = the nth nonnegative integer whose digits' harmonic
 * mean is an integer.  The function scans integers and checks the property
 * that (n * Πd) / Σ(Π d except one) is integral, treating a zero denominator
 * as giving the value 0. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/harmonic_integer.h"

/* cache of discovered terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static seq_t next_candidate = 0;

/* a(n) – nth term of the sequence (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* safety: offset is 1 */

    while (cache_size < (size_t)n) {
        if (harmonic_integer(next_candidate)) {
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* out‑of‑memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}

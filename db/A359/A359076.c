/*
 * A359076  Numbers that have at least two proper divisors with an equal sum
 *          of digits.
 * https://oeis.org/A359076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate numbers that have at least two proper divisors whose sums of digits are equal. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/seq_realloc.h"

/* ---------- helper: test whether a 64‑bit integer has the required divisors ---------- */
static bool has_equal_digit_sum_divisors(uint64_t n)
{
    /* array covering all possible digit sums for 64‑bit numbers */
    unsigned char seen[400] = {0};

    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        uint64_t q = n / d;

        /* proper divisor d (d < n because n > 1) */
        int s = (int)digit_sum((seq_t)d);
        if (seen[s])
            return true;
        seen[s] = 1;

        /* proper divisor q, distinct from d and n */
        if (q != d && q != n) {
            s = (int)digit_sum((seq_t)q);
            if (seen[s])
                return true;
            seen[s] = 1;
        }
    }
    return false;
}

/* ---------- cache for computed terms ---------- */
static seq_t *seq_cache = NULL;     /* dynamic array of terms */
static size_t cache_cap = 0;        /* allocated capacity */
static size_t cache_size = 0;       /* number of terms stored */
static uint64_t next_candidate = 2; /* next integer to test */

/* ---------- main sequence routine ---------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* behaviour below offset does not matter */
        return 0;

    /* expand cache until we have n terms */
    while (cache_size < (size_t)n) {
        if (has_equal_digit_sum_divisors(next_candidate)) {
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = seq_realloc(seq_cache, cache_cap, new_cap);
                if (!tmp)
                    abort();                       /* unrecoverable allocation failure */
                seq_cache = tmp;
                cache_cap = new_cap;
            }
            seq_cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return seq_cache[n - 1];
}

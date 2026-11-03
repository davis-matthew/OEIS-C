/*
 * A273362  Numbers n such that the decimal number concat(7,n) is a square.
 * https://oeis.org/A273362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 3056, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 54 terms through n = 8115 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that concat(7,n) is a perfect square.
   Computation: iterate through successive squares m², keep those whose
   most-significant decimal digit is 7, and whose remaining (d‑1) digits
   form an integer of exactly d‑1 digits (no leading zeros).  Those
   integers are the terms of the sequence.  Cached results allow
   O(1) access after the first O(n) computation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for offset 0 */

    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* global square counter */
    static idx_t m = 0;                  /* last m processed */

    /* grow cache until we have at least n entries */
    while (cache_len < (size_t)n)
    {
        /* enlarge cache if needed */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? 2 * cache_cap : 16;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)
            {
                mem_free(cache);
                cache = NULL;
                cache_len = cache_cap = 0;
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute next square */
        ++m;
        seq_t sq = (seq_t)m * (seq_t)m;        /* m² */

        int d = num_digits(sq);                 /* number of decimal digits */
        if (d <= 1) continue;                  /* cannot start with 7 */

        seq_t pow = pow10((idx_t)(d - 1));     /* 10^(d-1) */

        seq_t lead = sq / pow;
        if (lead == 7)
        {
            seq_t val = sq - ((seq_t)7) * pow; /* n = suffix after leading 7 */

            /* reject if val has fewer than d-1 digits (leading zeros) */
            seq_t min_pow = pow / 10;          /* 10^(d-2) */
            if (min_pow == 0 || val < min_pow)
                continue;

            cache[cache_len++] = val;          /* store valid term */
        }
    }

    return cache[n - 1];
}

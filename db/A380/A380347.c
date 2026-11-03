/*
 * A380347  a(1) = 1, a(2) = 3; thereafter, a(n) is the smallest nonnegative
 *          integer not yet in the sequence such that both a(n-2) + a(n-1) +
 *          a(n) and their concatenation are primes.
 * https://oeis.org/A380347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A380347:
 *  a(1)=1, a(2)=3; thereafter a(n) is the smallest non‑negative integer not yet
 *  in the sequence such that both the sum a(n-2)+a(n-1)+a(n) and the decimal
 *  concatenation of a(n-2), a(n-1), a(n) are prime.
 *  This routine builds the sequence on demand and memoises the values.
 *  It works for any seq_t/idx_t combination provided by the build
 *  (e.g. 8/16/32/64/128‑bit or arbitrary‑precision).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>          /*  malloc, realloc, abort  */
#include <limits.h>          /*  SIZE_MAX  */

/*  Utilities supplied by the build.  */
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

/*  Main routine.  */
seq_t a(idx_t n)
{
    /*  1‑based caching of computed terms.  */
    static seq_t *cache   = NULL;   /*  array  */
    static size_t  len    = 0;     /*  number of stored terms  */
    static size_t  cap    = 0;     /*  allocated capacity     */

    /*  Expand the cache to hold n terms.  */
    if (n > cap) {
        size_t new_cap = cap ? cap * 2 : 4;
        while (n > new_cap) new_cap *= 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();          /*  allocation failed  */
        cache = tmp;
        cap   = new_cap;
    }

    /*  Initialise the first two terms.  */
    if (len == 0) {
        cache[0] = (seq_t)1;
        len = 1;
        if (n == 1) return cache[0];
    }
    if (len == 1) {
        cache[1] = (seq_t)3;
        len = 2;
        if (n == 2) return cache[1];
    }

    /*  Compute terms up to n.  */
    while (len < (size_t)n) {
        seq_t prev2 = cache[len-2];
        seq_t prev1 = cache[len-1];
        seq_t next  = 0;          /*  placeholder  */

        for (uint64_t cand = 0; ; ++cand) {
            seq_t candidate = (seq_t)cand;

            /*  Skip if candidate already used.  */
            bool used = false;
            for (size_t i = 0; i < len; ++i) {
                if (cache[i] == candidate) { used = true; break; }
            }
            if (used) continue;

            /*  Sum must be prime.  */
            seq_t sum = prev2 + prev1 + candidate;
            if (!isPrimeGeneric(sum)) continue;

            /*  Concatenation must be prime.  */
            seq_t temp    = concat_parts((idx_t)prev2, (idx_t)prev1);
            seq_t concat  = concat_parts((idx_t)temp,   (idx_t)candidate);
            if (!isPrimeGeneric(concat)) continue;

            /*  Found the required term.  */
            next = candidate;
            break;
        }

        cache[len++] = next;
    }

    return cache[n-1];
}

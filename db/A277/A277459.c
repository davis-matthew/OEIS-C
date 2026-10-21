/*
 * A277459  Decades with the same semiprime pattern as the next decade.
 * https://oeis.org/A277459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = starting decade of the nth pair of consecutive decades that
 * have the same semiprime pattern. The pattern of a decade d
 * (numbers 10*d … 10*d+9) is a 10‑bit mask with bit i set iff
 * 10*d+i is a semiprime (product of exactly two primes, multiplicities
 * counted). The function returns the decade d of the first decade in
 * the pair. */

#include <stdint.h>
#include <stdlib.h>

/* Return true iff m is a semiprime (product of exactly two primes,
 * counting multiplicity). */
static int is_semiprime(uint64_t m)
{
    if (m < 4) return 0;          /* 0,1,2,3 are not semiprime */
    int count = 0;
    while ((m & 1ULL) == 0) {     /* factor 2 while possible */
        m >>= 1;
        ++count;
        if (count > 2) return 0;
    }
    for (uint64_t p = 3; p * p <= m; p += 2) {
        while (m % p == 0) {
            m /= p;
            ++count;
            if (count > 2) return 0;
        }
    }
    if (m > 1) ++count;           /* remaining prime factor */
    return count == 2;
}

/* Return a 10‑bit mask representing the semiprime pattern of decade d
 * (numbers 10*d .. 10*d+9). Bit i (0‑based) is set iff 10*d+i is a
 * semiprime. */
static uint16_t decade_pattern(idx_t d)
{
    uint16_t mask = 0;
    uint64_t base = (uint64_t)d * 10ULL;
    for (int i = 0; i < 10; ++i) {
        if (is_semiprime(base + i))
            mask |= (uint16_t)(1U << i);
    }
    return mask;
}

/* Return the nth term of sequence A277459 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but avoid UB */

    /* Static state for memoisation. */
    static seq_t *cache      = NULL; /* array of found decades */
    static size_t cache_len  = 0;    /* number of cached results */
    static size_t cache_cap  = 0;    /* current capacity of cache */
    static idx_t  next_d     = 1;    /* next decade to examine */
    static uint16_t prev_mask = 0;   /* pattern of decade (next_d-1) */
    static int initialized = 0;

    if (!initialized) {
        prev_mask = decade_pattern(0);   /* pattern of decade 0 */
        next_d = 1;
        initialized = 1;
    }

    /* Extend cache until we have at least n entries. */
    while (cache_len < (size_t)n) {
        uint16_t cur_mask = decade_pattern(next_d);
        if (prev_mask == cur_mask) {
            /* Pattern matched: decade (next_d-1) starts the pair. */
            seq_t val = (seq_t)(next_d - 1);
            /* Grow the cache array if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {          /* allocation failed */
                    /* In this unlikely event return 0. */
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        prev_mask = cur_mask;
        ++next_d;
    }

    return cache[n - 1];
}

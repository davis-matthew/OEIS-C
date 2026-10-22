/*
 * A342944  Numbers m such that d(1)^0 + d(2)^1 + ... + d(k)^(k-1) = d(1)! +
 *          d(2)! + ... + d(k)!, where d(i), i=1..k, are the digits of m.
 * https://oeis.org/A342944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A342944: numbers m such that
   ∑_{i=1}^k d_i^{i-1} = ∑_{i=1}^k d_i!,
   where d_i are the decimal digits of m. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Factorials of the digits 0…9 */
static const uint64_t fact[10] = {
    1ULL, 1ULL, 2ULL, 6ULL, 24ULL,
    120ULL, 720ULL, 5040ULL,
    40320ULL, 362880ULL
};

/* Test whether a number satisfies the defining equality.
   The test is performed using 64‑bit arithmetic which is
   sufficient for all values occurring in the search. */
static bool satisfies(uint64_t m)
{
    uint8_t digs[20];   /* enough for any 64‑bit integer */
    size_t len = 0;

    /* Extract digits in reverse order */
    uint64_t t = m;
    do {
        digs[len++] = (uint8_t)(t % 10);
        t /= 10;
    } while (t != 0);

    uint64_t sum_pow = 0, sum_fact = 0;

    /* Process digits from most significant to least */
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = digs[len - 1 - i];   /* leftmost digit first */

        /* Compute d^i */
        uint64_t pow_val = 1;
        for (size_t e = 0; e < i; ++e)
            pow_val *= d;

        sum_pow += pow_val;
        sum_fact += fact[d];
    }
    return sum_pow == sum_fact;
}

/* Cached sequence values for arbitrary access order */
static seq_t   *cache     = NULL;   /* array of found terms   */
static size_t  cache_len = 0;       /* number of stored terms */
static size_t  cache_cap = 0;       /* allocated capacity     */
static idx_t   last_chk  = 0;       /* last integer examined   */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for 0, return 0 */

    /* Generate terms until we have at least n entries */
    while (cache_len < (size_t)n) {
        ++last_chk;
        if (satisfies((uint64_t)last_chk)) {
            /* Grow storage if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0 ? 1 : cache_cap * 2);
                seq_t *tmp = (seq_t *)realloc(cache,
                                              new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_chk;
        }
    }
    return cache[(size_t)n - 1];
}

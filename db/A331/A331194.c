/*
 * A331194  Numbers whose last digit is the number of their distinct prime
 *          factors.
 * https://oeis.org/A331194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 6412, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A331194: Numbers whose last digit is the number of distinct prime factors.
   The code builds the sequence by sieving the number of distinct primes up
   to a sufficient bound, stores all qualifying numbers in a static cache,
   and extends that cache when a larger index is requested. */

#include <stdlib.h>
#include <stdbool.h>

/* Append a value to a dynamic array of seq_t; the array is reallocated as needed. */
static void append_seq(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len >= *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!tmp) exit(1);               /* allocation failed */
        *arr   = tmp;
        *cap   = newcap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Sequence A331194 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* no term for n = 0 */

    /* static cache of previously computed terms */
    static seq_t *cache        = NULL;   /* array of terms   */
    static size_t cached_len   = 0;      /* number of terms  */
    static idx_t  cached_limit = 0;      /* sieve upper bound used */

    /* If we already have enough terms, return the requested value. */
    if (cached_len >= n) return cache[n-1];

    /* Start with an initial bound; increase as necessary. */
    idx_t limit = (cached_limit > 0) ? cached_limit : 200000;
    if (limit < 200000) limit = 200000;

    while (1) {
        /* Allocate and zero the array that stores the number of distinct primes. */
        unsigned char *cnt =
            (unsigned char *)calloc((size_t)(limit + 1), sizeof(unsigned char));
        if (!cnt) return 0;             /* allocation failure */

        /* Sieve: for each prime add 1 to each multiple. */
        for (idx_t p = 2; p <= limit; ++p) {
            if (cnt[p] == 0) {           /* p is prime */
                for (idx_t j = p; j <= limit; j += p)
                    ++cnt[j];
            }
        }

        /* Build a fresh cache of qualifying numbers. */
        seq_t *newcache   = NULL;
        size_t newlen     = 0;
        size_t newcap     = 0;

        for (idx_t i = 2; i <= limit; ++i) {
            unsigned char last = (unsigned char)(i % 10);
            if (cnt[i] == last) {
                append_seq(&newcache, &newlen, &newcap, (seq_t)i);
            }
        }

        free(cnt);

        /* Replace the old cache. */
        free(cache);
        cache        = newcache;
        cached_len   = newlen;
        cached_limit = limit;

        if (cached_len >= (size_t)n) break;  /* we have enough terms */

        /* Not enough terms yet: double the search bound. */
        limit <<= 1;
    }

    return cache[n-1];
}

/*
 * A381546  Numbers with an odd number of abundant divisors.
 * https://oeis.org/A381546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A381546: Numbers with an odd number of abundant divisors.
   This implementation sieves the integers up to the smallest limit
   that guarantees at least `n` terms and stores all terms in a
   dynamically allocated array.  The sieve recomputes everything
   from scratch whenever a larger limit is required.
   The algorithm uses least prime factor sieving to compute
   the sum of divisors and then toggles parity for each multiple
   of an abundant number.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Global cache of sequence terms and limits. */
static seq_t *seq_cache = NULL;      /* array of terms */
static size_t seq_len   = 0;          /* number of terms stored   */
static idx_t max_value  = 0;          /* largest numeric value
                                         that has been sieved    */

/* Forward declaration of the helper that rebuilds the cache. */
static void rebuild_cache(idx_t limit);

/* Public sequence function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined; guard for safety */

    /* Ensure that the cache contains at least n terms. */
    while (seq_len < (size_t)n)
    {
        /* Start with a reasonable limit and double it each time
           until enough terms are found. */
        idx_t new_limit;
        if (max_value == 0)
            new_limit = 2000;      /* arbitrary initial bound */
        else
            new_limit = max_value * 2;
        rebuild_cache(new_limit);
    }

    if (n > seq_len) return 0;     /* safety fall‑back */
    return seq_cache[(size_t)n - 1];
}

/* ----------------------------------------------------------------------
   Rebuilds the entire cache up to the given numeric `limit`.
   The function is called only when a larger limit is needed.
   ---------------------------------------------------------------------- */
static void rebuild_cache(idx_t limit)
{
    if (limit < 12) limit = 12;    /* smallest abundant number */

    /* Allocate arrays for SPF, abundant flag and parity flag. */
    size_t sz = (size_t)limit + 1;

    uint32_t *spf = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!spf) return;
    for (size_t i = 0; i < sz; ++i) spf[i] = (uint32_t)i;

    /* Build SPF table. */
    for (idx_t i = 2; i * i <= limit; ++i)
    {
        if (spf[(size_t)i] == (uint32_t)i)   /* i is prime   */
        {
            for (idx_t j = i * i; j <= limit; j += i)
            {
                if (spf[(size_t)j] == (uint32_t)j)
                    spf[(size_t)j] = (uint32_t)i;
            }
        }
    }

    unsigned char *is_abundant =
        (unsigned char *)calloc(sz, sizeof(unsigned char));
    unsigned char *odd_parity =
        (unsigned char *)calloc(sz, sizeof(unsigned char));
    if (!is_abundant || !odd_parity)
    {
        free(spf);
        free(is_abundant);
        free(odd_parity);
        return;
    }

    /* Determine abundant numbers up to limit. */
    for (idx_t n = 12; n <= limit; ++n)
    {
        idx_t tmp = n;
        unsigned __int128 sigma = 1;

        while (tmp > 1)
        {
            uint32_t p = spf[(size_t)tmp];
            unsigned int exp = 0;
            unsigned __int128 pow = 1;

            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
                pow *= p;
            }

            /* (p^{exp+1}-1)/(p-1) */
            unsigned __int128 term =
                (pow * (unsigned __int128)p - 1) / (p - 1);
            sigma *= term;
        }

        if (sigma > (unsigned __int128)2 * (unsigned __int128)n)
            is_abundant[(size_t)n] = 1;
    }

    /* Toggle parity for each multiple of every abundant number. */
    for (idx_t a = 12; a <= limit; ++a)
    {
        if (is_abundant[(size_t)a])
        {
            for (idx_t m = a; m <= limit; m += a)
                odd_parity[(size_t)m] ^= 1;
        }
    }

    /* Count how many terms we will store to allocate exactly. */
    size_t count = 0;
    for (idx_t k = 1; k <= limit; ++k)
        if (odd_parity[(size_t)k]) ++count;

    /* Allocate new cache. */
    seq_t *new_cache = (seq_t *)malloc(count * sizeof(seq_t));
    if (!new_cache)
    {
        free(spf);
        free(is_abundant);
        free(odd_parity);
        return;
    }

    /* Fill cache. */
    size_t pos = 0;
    for (idx_t k = 1; k <= limit; ++k)
    {
        if (odd_parity[(size_t)k])
        {
            new_cache[pos++] = (seq_t)k;
        }
    }

    /* Replace old cache. */
    free(seq_cache);
    seq_cache = new_cache;
    seq_len   = count;
    max_value = limit;

    /* Clean up temporary arrays. */
    free(spf);
    free(is_abundant);
    free(odd_parity);
}

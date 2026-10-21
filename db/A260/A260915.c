/*
 * A260915  Prime number days: prime numbers that can be interpreted as an ISO
 *          date.
 * https://oeis.org/A260915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9504, u64 n <= 9504, u128 n <= 9504,
 *            big any n
 * Helpers:   is_prime, is_valid_iso_date
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th prime that can be written as an ISO date
   YYYYMMDD. The function uses a simple incremental search; a cache of
   already found terms is maintained for efficiency. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Provided by the build environment (see the list of utilities). */
#include "../utils/is_prime.h"
#include "../utils/is_valid_iso_date.h"

/* Cache of already discovered sequence terms. */
static seq_t *cache      = NULL;    /* array of found terms */
static size_t cache_cap  = 0;       /* current capacity of cache */
static size_t cache_cnt  = 0;       /* number of stored terms */

/* Next odd candidate to test for primality and validity.  The first
   odd ISO‑date prime is 10000103. */
static uint64_t next_candidate = 10000103;

/* Ensure that the cache can hold at least `need` elements. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 128;
    while (new_cap < need)
        new_cap <<= 1;              /* double until large enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* allocation failure – terminate */

    cache      = tmp;
    cache_cap  = new_cap;
}

/* Find the next prime that represents a valid ISO date.  The caller
   assumes that the overall sequence has a finite maximum (99991207). */
static seq_t find_next_prime_day(void)
{
    for (;;) {
        uint64_t cand = next_candidate;
        /* advance to the next odd number (prime > 2 is odd). */
        next_candidate += 2;

        if (cand > 99991207)          /* safety; the sequence ends here */
            return 0;                 /* caller should never request beyond this */

        if (!is_valid_iso_date(cand))
            continue;

        if (is_prime((idx_t)cand))
            return (seq_t)cand;
    }
}

/*
 * seq_t a(idx_t n)
 *
 * Returns the n‑th term (1‑based) of the sequence of prime numbers
 * that can be interpreted as ISO dates.  The function memoises
 * previously computed terms so that calls are fast and order‑independent.
 */
seq_t a(idx_t n)
{
    if (n == 0)                 /* safety; OEIS offset is 1 */
        return 0;

    /* Prepare cache to hold at least n terms. */
    ensure_cache((size_t)n);

    /* Generate terms until we have at least n of them. */
    while (cache_cnt < n) {
        seq_t val = find_next_prime_day();
        cache[cache_cnt++] = val;
    }

    return cache[(size_t)n - 1];
}

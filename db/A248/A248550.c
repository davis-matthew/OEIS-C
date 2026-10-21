/*
 * A248550  Numbers n such that the smallest prime divisor of n^2+1 is 73.
 * https://oeis.org/A248550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 310, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th positive integer n such that the
   smallest prime divisor of n^2 + 1 is 73.  The function scans the
   positive integers, keeps only those n with n^2 ≡ –1 (mod 73),
   rejects any n for which a prime < 73 divides n^2 + 1, and
   stores the found terms in a static cache.  Subsequent calls reuse
   the cache, so any order of queries is handled efficiently. */

/*  Required headers  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*  The sequence function  */
seq_t a(idx_t n)
{
    /* Reject non‑positive indices (offset is 1) */
    if (n == 0)
        return 0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored */
    static idx_t last_checked = 0; /* largest integer examined so far */
    static size_t cap = 0;         /* allocated capacity of cache */

    /* Small primes < 73 that can divide n^2+1 */
    static const int primes[] = {2,5,13,17,29,37,41,53,61};
    const size_t prime_cnt = sizeof(primes) / sizeof(primes[0]);

    /* Generate terms until we reach the requested index */
    while (cache_size < n)
    {
        /* Move to the next integer */
        ++last_checked;

        /* Only numbers n ≡ 27 or 46 (mod 73) can satisfy n^2 ≡ –1 (mod 73) */
        idx_t mod73 = last_checked % 73;
        if (mod73 != 27 && mod73 != 46)
            continue;

        /* Check that no prime < 73 divides n^2+1 */
        bool ok = true;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            idx_t p = (idx_t)primes[i];
            idx_t r = last_checked % p;
            if (((r * r + (idx_t)1) % p) == 0)
            {
                ok = false;
                break;
            }
        }
        if (!ok)
            continue;  /* reject this n */

        /* Accept the term: resize cache if needed */
        if (cache_size == cap)
        {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)  /* allocation failure – return 0 as a safe fallback */
                return 0;
            cache = tmp;
            cap   = new_cap;
        }

        /* Store the term in the cache */
        cache[cache_size++] = (seq_t)last_checked;
    }

    /* Return the requested term (offset 1 → index n‑1 in cache) */
    return cache[n - 1];
}

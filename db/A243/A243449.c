/*
 * A243449  Primes of the form n^2 + 14.
 * https://oeis.org/A243449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime of the form m^2 + 14 (search)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 13, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Cached primes of the form m^2 + 14. */
static seq_t *prime_cache = NULL;   /* array of found primes          */
static size_t cache_size = 0;       /* number of primes cached        */
static size_t cache_cap   = 0;       /* capacity of the array          */
static idx_t  last_m      = 0;       /* last m that was processed      */

/* Ensure the cache has room for at least `need` elements. */
static void
ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_buf = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!new_buf)
        abort();                     /* allocation failed */
    prime_cache = new_buf;
    cache_cap   = new_cap;
}

/* Return the nth prime of the form m^2 + 14 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)         /* the sequence is defined from index 1 */
        return (seq_t)0;

    /* Grow the cache until we have at least n primes. */
    if (cache_size < n) {
        ensure_cache_capacity(n);
        idx_t m;
        for (m = last_m + 1; cache_size < n; ++m) {
            seq_t m_seq = (seq_t)m;
            seq_t cand  = m_seq * m_seq + (seq_t)14;
            if (is_prime_big(cand))
                prime_cache[cache_size++] = cand;
        }
        last_m = m - 1;   /* last m processed */
    }

    return prime_cache[n - 1];
}

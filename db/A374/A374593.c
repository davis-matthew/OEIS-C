/*
 * A374593  Numbers k such that k - rad(k) + 1 is prime, where rad(k) is the
 *          radical A007947(k).
 * https://oeis.org/A374593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that k - rad(k) + 1 is prime
 * Range:     u8 n <= 59, u16 n <= 6272, u32 n <= 7379, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime_big, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 4, a(2) = 8, a(3) = 9, a(4) = 12, ... */
/* The function caches the computed terms so that each value is only
   evaluated once, independent of call order. */

#include <stddef.h>                 /* size_t */
#include "../utils/rad.h"           /* seq_t rad(idx_t m); */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n); */
#include "../utils/append_candidate.h" /* append_candidate(seq_t **, size_t *, size_t *, seq_t); */

static seq_t *g_cache = NULL;   /* dynamic array of found terms (k values) */
static size_t g_len   = 0;       /* number of terms stored */
static size_t g_cap   = 0;       /* current capacity of g_cache */
static idx_t  g_next  = 1;       /* next integer to examine */

/* Extend the cache until it contains at least n terms.  */
static void
ensure_cache(size_t n)
{
    while (g_len < n) {
        idx_t k = g_next++;
        seq_t radk = rad(k);
        seq_t val  = (seq_t)k - radk + 1;
        if (is_prime_big(val))
            append_candidate(&g_cache, &g_len, &g_cap, (seq_t)k);
    }
}

/* Public function: return the n‑th term (1‑based index).  */
seq_t
a(idx_t n)
{
    if (n == 0)                    /* offset is 1; undefined for 0 */
        return 0;
    if (g_len < (size_t)n)         /* compute more terms if necessary */
        ensure_cache((size_t)n);
    return g_cache[(size_t)n - 1];
}

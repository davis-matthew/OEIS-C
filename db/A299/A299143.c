/*
 * A299143  a(n) is the least k > n such that gcd(k,n) > 1 and gcd(k+1,n+1) >
 *          1.
 * https://oeis.org/A299143
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least k > n such that gcd(k,n) > 1 and gcd(k+1,n+1)
 *            > 1. It satisfies: a(2k+1) = a(2k)+1 for k>0, and for odd primes
 *            p, a(p)=3p
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses memoisation and a direct search for even indices.
   For odd n>2 it reuses a(n-1)+1, thus avoiding repeated searches. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;   /* Cached results for indices >=2 */
static bool   *seen  = NULL;   /* true if cache[n] contains a valid value */
static size_t  cap   = 0;      /* current allocation size of cache and seen arrays */

/* Ensure internal arrays can hold index n.  Allocate more space as needed. */
static void ensure_capacity(idx_t n)
{
    if (n < cap)
        return;
    size_t newcap = cap ? cap << 1 : 64;
    while (newcap <= (size_t)n)
        newcap <<= 1;
    seq_t *newc = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    bool  *news = (bool  *)realloc(seen,  newcap * sizeof(bool));
    if (!newc || !news)
        abort();  /* memory allocation failed */

    cache = newc;
    seen  = news;
    cap   = newcap;
    /* No need to initialise new cache entries; they are only read when seen[n] is true. */
}

/* Recursively compute a(n) with memoisation.  n must be >= 2. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* undefined for indices < 2 */
    ensure_capacity(n);
    if (seen[n])
        return cache[n];

    seq_t result;
    if ((n & 1u) == 1u) {          /* odd n >= 3 */
        result = a(n - 1) + 1;
    } else {                       /* even n: brute‑force search */
        seq_t n_seq  = (seq_t)n;
        seq_t n1_seq = n_seq + 1;
        idx_t k = n + 1;           /* candidate k, always > n */
        for (;; ++k) {
            seq_t k_seq = (seq_t)k;
            if (gcd(k_seq, n_seq) != (seq_t)1 &&
                gcd(k_seq + 1, n1_seq) != (seq_t)1) {
                result = k_seq;
                break;
            }
        }
    }

    cache[n] = result;
    seen[n]  = true;
    return result;
}

/*
 * A379738  Numbers k such that prime(k) and prime(k) + 9*k are anagrams.
 * https://oeis.org/A379738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   anagram_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A379738:
   a(n) = k such that prime(k) and prime(k) + 9*k are anagrams (in decimal).
   Indices are 1‑based.  a(1) = 8, a(2) = 55, …   see the OEIS entry for
   the first 24 terms.  The code is memoised and does not assume any
   ordering of the calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/anagram_seq.h"

/* ---------- cache for the k values ----------------------------------- */
static seq_t *k_cache = NULL;   /* array of results (k) */
static size_t cache_len = 0;    /* how many terms are cached */
static size_t cache_cap = 0;    /* allocated capacity */

/* Grow the cache to hold at least new_len elements */
static void grow_cache(size_t new_len)
{
    if (cache_cap < new_len) {
        size_t new_cap = (cache_cap ? cache_cap << 1 : 8);
        if (new_cap < new_len) new_cap = new_len;
        seq_t *tmp = (seq_t *)realloc(k_cache, new_cap * sizeof(seq_t));
        k_cache = tmp;
        cache_cap = new_cap;
    }
}

/* ---------- global prime enumeration state --------------------------- */
static idx_t prime_index = 0;     /* number of primes seen so far (1‑based) */
static idx_t next_candidate = 2;  /* integer currently being tested for primality */

/* ---------- main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n < 1, just return 0 */

    /* Extend the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        /* Skip non‑primes */
        while (!is_prime(next_candidate)) {
            ++next_candidate;
        }

        /* Found the (prime_index+1)-th prime.  Update prime_index. */
        ++prime_index;

        seq_t prime = (seq_t)next_candidate;
        seq_t k     = (seq_t)prime_index;           /* current k */
        seq_t val   = prime + ( (seq_t)9 * k );     /* prime + 9*k */

        /* If anagram, record k */
        if (anagram_seq(prime, val)) {
            grow_cache(cache_len + 1);
            k_cache[cache_len] = k;
            ++cache_len;
        }

        /* Advance to next integer */
        ++next_candidate;
    }

    return k_cache[n - 1];      /* cache is 0‑based, input n is 1‑based */
}

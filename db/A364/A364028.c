/*
 * A364028  Odd squarefree semiprimes s = p*q such that p + q and p - q are
 *          not squarefree.
 * https://oeis.org/A364028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   append_candidate, is_prime_big, is_squarefree,
 *            smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th odd squarefree semiprime s = p*q such that
   both p+q and |p−q| are not squarefree.
   Sequence is OEIS A364028.  Computation is done by a simple
   forward search with memoization. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/smallest_prime_factor.h"   /* uint64_t smallest_prime_factor(uint64_t n); */
#include "../utils/is_squarefree.h"           /* bool is_squarefree(seq_t x); */
#include "../utils/is_prime_big.h"            /* bool is_prime_big(seq_t n); */
#include "../utils/append_candidate.h"        /* void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val); */

seq_t a(idx_t n)
{
    /* static memoization of sequence values */
    static seq_t *cache = NULL;          /* array of terms */
    static size_t cache_len = 0;         /* number of stored terms */
    static size_t cache_cap = 0;         /* allocated capacity */
    static uint64_t last_checked = 1;    /* largest integer inspected so far */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        uint64_t m = last_checked + 1;   /* next integer to test */

        /* Find the next valid odd semiprime */
        for (;;) {
            /* Skip even numbers */
            if ((m & 1ULL) == 0) { m++; continue; }

            /* Factor m; if m is prime or has no factor, skip */
            uint64_t p = smallest_prime_factor(m);
            if (p == 0 || p == m) { m++; continue; }

            uint64_t q = m / p;
            if (p == q || q == 0) { m++; continue; }   /* square of a prime */

            /* Ensure both factors are (odd) primes */
            if (!is_prime_big((seq_t)q)) { m++; continue; }

            /* Check squarefree conditions on sum and difference */
            seq_t sum = (seq_t)p + (seq_t)q;
            seq_t diff = (p > q) ?
                          (seq_t)(p - q) :
                          (seq_t)(q - p);
            if (is_squarefree(sum) || is_squarefree(diff)) { m++; continue; }

            /* Found a valid term */
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)m);
            break;
        }

        last_checked = m;
    }

    return cache[n - 1];
}

/*
 * A364141  Products k of 4 distinct primes (or tetraprimes) such that k has
 *          no squarefree neighbors.
 * https://oeis.org/A364141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   count_distinct_primes, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A364141:
   a(n) = the n-th positive integer k that
          • k is a product of exactly 4 distinct primes (k is squarefree and omega(k)=4)
          • k-1 is NOT squarefree
          • k+1 is NOT squarefree
   (offset 1).  The function finds terms by scanning upward, caching already found
   terms, and re‑using that cache for any subsequent calls. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"
#include "../utils/count_distinct_primes.h"

/* Return the n-th term of A364141 (offset 1). */
seq_t a(idx_t n)
{
    /* Cache of already found terms. */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0, terms_len = 0;
    static seq_t candidate = 2;  /* next integer to be tested */

    if (n == (idx_t)0) return (seq_t)0;   /* safety – offset is 1 */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        /* Find the next suitable candidate by scanning upwards. */
        for ( ; ; ++candidate) {
            /* Candidate must be squarefree. */
            if (!is_squarefree(candidate)) continue;

            /* Since it is squarefree, count_distinct_primes gives the
               number of distinct prime factors.  We need exactly 4. */
            if (count_distinct_primes((uint64_t)candidate) != 4) continue;

            /* Both neighbors must NOT be squarefree. */
            if (!is_squarefree(candidate - 1) && !is_squarefree(candidate + 1)) {
                /* Ensure capacity for the new term. */
                if (terms_len == terms_cap) {
                    size_t new_cap = terms_cap == 0 ? 4 : terms_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* out of memory */
                    terms = tmp;
                    terms_cap = new_cap;
                }
                /* Store the candidate. */
                terms[terms_len++] = candidate;
                /* Move past this candidate for the next search. */
                ++candidate;
                break;  /* proceed to the next term */
            }
        }
    }

    /* Return the requested term (offset 1). */
    return terms[n - 1];
}

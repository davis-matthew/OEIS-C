/*
 * A241817  Semiprimes sp such that sp-3 is prime.
 * https://oeis.org/A241817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 914, u32 n <= 915, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Semantic description:
   a(n) = 2p where p is the n-th prime such that 2p-3 is also prime. */

/* Includes */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

/* Cached data */
static seq_t   *terms        = NULL;   /* array of 2p values          */
static size_t   terms_len    = 0;       /* number of computed terms   */
static size_t   terms_cap    = 0;       /* current array capacity     */
static uint64_t last_prime   = 1;       /* last prime examined        */

/* Main function */
seq_t a(idx_t n)
{
    /* n == 0 is outside the defined offset */
    if (n == 0U) return 0U;

    /* Ensure we have enough terms for all requested indices */
    while (terms_len < (size_t)n)
    {
        /* Find next prime after last_prime */
        uint64_t p = next_prime(last_prime);
        last_prime = p;            /* remember for next call */

        /* Skip the trivial prime 2 (since 2*2-3 = 1 is not prime) */
        if (p < 2ULL) continue;

        /* sp = 2p   (always even and semiprime) */
        seq_t sp = (seq_t)p * 2U;

        /* Test whether sp-3 is prime */
        seq_t test = sp - 3U;
        if (test > 1U && is_prime_big(test))
        {
            /* Grow terms array if necessary */
            if (terms_len >= terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap << 1 : 16U;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp)           /* allocation failed */
                {
                    free(terms);
                    terms = NULL;
                    terms_len = terms_cap = 0U;
                    return 0U;    /* safe failure */
                }
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = sp;
        }
    }

    return terms[n - 1U];
}

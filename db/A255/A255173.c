/*
 * A255173  Numbers k such that 1+prime(k) and 1+prime(k+1) are the product of
 *          the same number of primes.
 * https://oeis.org/A255173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   count_prime_factors, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that 1+prime(k) and 1+prime(k+1) have the same number
   of prime factors (counted with multiplicity). */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first such k.  Return the nth stored term. */
    if (n == 0) return 0; /* prevent invalid access, not part of the sequence */

    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static idx_t last_k = 0;

    /* Ensure we have at least n terms computed.  The search moves only forward,
       so repeated calls reuse previous work. */
    while (terms_len < (size_t)n) {
        ++last_k;

        seq_t pk  = (seq_t)get_prime(last_k);
        seq_t pk1 = (seq_t)get_prime(last_k + 1);

        seq_t c1 = count_prime_factors(pk + (seq_t)1);
        seq_t c2 = count_prime_factors(pk1 + (seq_t)1);

        if (c1 == c2) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = last_k;
        }
    }

    return terms[n - 1];
}

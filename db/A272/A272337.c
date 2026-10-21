/*
 * A272337  Numbers such that antisigma(n) mod sigma(n) = d(n), where
 *          antisigma(n) is the sum of the numbers less than n that do not
 *          divide n, sigma(n) is the sum of the divisors of n and d(n) is the
 *          number of divisors of n.
 * https://oeis.org/A272337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer N such that (N*(N+1)/2) mod sigma(N) =
 *            d(N), where sigma(N) is the sum of divisors of N and d(N) is the
 *            number of divisors of N
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"

/* 
   Static state: terms[] holds the sequence values found so far.
   We compute terms on demand up to the largest requested index.
*/
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static idx_t next_test = 2;   /* smallest integer to test next */

seq_t a(idx_t n) {
    /* Ensure we have at least n terms in the table. */
    while (terms_len < n) {
        /* Check the current integer for the defining property. */
        seq_t m = (seq_t)next_test;
        seq_t tri = m * (m + 1) / 2;               /* N*(N+1)/2                  */
        seq_t sigma = sigma_of(next_test);         /* sum of divisors of N        */
        seq_t d = (seq_t)divisor_count_u64((uint64_t)next_test); /* number of divisors */

        if ((tri % sigma) == d) {
            /* Store the qualifying integer. */
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 128;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)next_test;
        }
        /* Advance to the next candidate. */
        next_test++;
    }
    /* Return the requested term. */
    return terms[n - 1];
}

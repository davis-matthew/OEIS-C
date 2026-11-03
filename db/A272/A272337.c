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
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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
        seq_t d = (seq_t)num_divisors((uint64_t)next_test); /* number of divisors */

        if ((tri % sigma) == d) {
            /* Store the qualifying integer. */
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 128;
                terms = mem_resize(seq_t, terms, terms_cap);
            }
            terms[terms_len++] = (seq_t)next_test;
        }
        /* Advance to the next candidate. */
        next_test++;
    }
    /* Return the requested term. */
    return terms[n - 1];
}

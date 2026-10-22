/*
 * A336687  Numbers m such that tau(sigma(m)) and sigma(tau(m)) both divide m,
 *          where tau(m) is the number of divisors function (A000005) and
 *          sigma(m) is the sum of divisors function (A000203).
 * https://oeis.org/A336687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m such that tau(sigma(m)) and
 *            sigma(tau(m)) both divide m, 1-indexed. The function searches
 *            incrementally and memoizes the sequence
 * Range:     u8 n <= 9, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    static seq_t *terms   = NULL;
    static size_t terms_len = 0, terms_cap = 0;
    static uint64_t tested = 0; /* last integer examined, m = tested */

    while (terms_len < n) {
        ++tested;
        uint64_t m = tested;

        /* tau(m) = number of divisors of m */
        unsigned tau_m = divisor_count_u64(m);

        /* sigma(m) = sum of divisors of m */
        seq_t sigma_m_seq = sigma_of(m);
        uint64_t sigma_m   = (uint64_t)sigma_m_seq;  /* sigma(m) <= 2m, fits in 64 bits */

        /* tau( sigma(m) ) */
        unsigned tau_sigma = divisor_count_u64(sigma_m);

        /* sigma( tau(m) ) */
        seq_t sigma_tau_seq = sigma_of(tau_m);
        uint64_t sigma_tau   = (uint64_t)sigma_tau_seq;

        if ((m % tau_sigma == 0) && (m % sigma_tau == 0)) {
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 4;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)m;
        }
    }
    return terms[n - 1];
}

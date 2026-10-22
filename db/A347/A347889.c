/*
 * A347889  Numbers k such that sigma(k) > 2*k and A003415(sigma(k)) < k,
 *          where A003415 is the arithmetic derivative, and sigma is the sum
 *          of divisors function.
 * https://oeis.org/A347889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A347889: numbers k such that sigma(k) > 2*k and A003415(sigma(k)) < k
   The function searches for the terms by testing successive integers.
   It keeps a cache of already found terms so that calls may arrive in any
   order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

/* Return the arithmetic derivative of m (A003415) */
static seq_t arithmetic_derivative(seq_t m)
{
    if (m <= (seq_t)1)
        return (seq_t)0;
    seq_t result = (seq_t)0;
    seq_t temp = m;
    for (idx_t p = (idx_t)2; (seq_t)p * (seq_t)p <= temp; ++p) {
        if (temp % (seq_t)p == (seq_t)0) {
            seq_t exp = (seq_t)0;
            while (temp % (seq_t)p == (seq_t)0) {
                temp /= (seq_t)p;
                exp += (seq_t)1;
            }
            result += (m / (seq_t)p) * exp;
        }
    }
    if (temp > (seq_t)1) {                /* remaining prime factor */
        result += (m / temp);              /* exponent is 1 */
    }
    return result;
}

/* External function required by the framework */
seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t last_checked = 0;

    /* Ensure the cache can hold at least n terms */
    while (terms_len < n) {
        /* Allocate / grow the array as needed */
        if (terms_len == terms_cap) {
            size_t new_cap = terms_cap == 0 ? 64 : terms_cap * 2;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* on allocation failure */
            terms = tmp;
            terms_cap = new_cap;
        }

        /* test next integer */
        ++last_checked;
        seq_t k = (seq_t)last_checked;
        seq_t sigma_k = sigma_of(last_checked);

        /* condition sigma(k) > 2*k */
        if (sigma_k <= (seq_t)2 * (seq_t)last_checked)
            continue;

        /* condition A003415(sigma(k)) < k */
        seq_t sigma_deriv = arithmetic_derivative(sigma_k);
        if (sigma_deriv < (seq_t)last_checked) {
            terms[terms_len++] = k;
        }
    }

    return terms[n - 1];
}

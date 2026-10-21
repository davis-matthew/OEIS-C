/*
 * A253474  Numbers n such that the difference between n^2 and largest prime
 *          less than n^2 is not prime.
 * https://oeis.org/A253474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth integer m such that m^2 minus the largest prime
 *            less than m^2 is not prime (i.e. the difference equals 1 or a
 *            composite number)
 * Range:     u8 n <= 2, u16 n <= 69, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* External linkage */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    /* next integer to test (start with 2) */
    static idx_t next_candidate = 2;

    /* zero is outside the sequence (offset 1). Return 0 to indicate error. */
    if (n == 0)
        return 0;

    /* Generate more terms until we have at least n of them */
    while (terms_len < n) {
        seq_t sq = (seq_t)next_candidate * (seq_t)next_candidate;

        /* Find the largest prime < sq */
        seq_t p = sq - (seq_t)1;
        while (!is_prime_big(p))
            p -= 1;

        seq_t diff = sq - p;

        /* If the difference is 1 or composite, store the term */
        if (diff == 1 || !is_prime_big(diff)) {
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 4;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = next_candidate;
        }

        ++next_candidate;
    }

    return terms[n - 1];
}

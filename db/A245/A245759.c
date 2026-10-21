/*
 * A245759  Primes p such that concatenation of p with its digit sum is also
 *          prime.
 * https://oeis.org/A245759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 16, u32 n <= 19, u64 n <= 8160, u128 n <= 8160,
 *            big any n
 * Helpers:   concat_parts, digit_sum, is_prime_big, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A245759: primes p such that concatenation of p with its digit sum is also prime.
   The function Returns the n-th such prime (with a(1) = 61). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/digit_sum.h"
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *terms = NULL;
    static size_t length = 0;   /* number of terms stored */
    static size_t capacity = 0; /* capacity of terms array */
    static uint64_t last_checked = 1; /* last integer examined for primality */

    if (n == 0)
        return 0; /* the sequence offset is 1 */

    /* generate terms until we have at least n */
    while (length < n) {
        /* find next prime after last_checked */
        last_checked = next_prime(last_checked + 1);
        seq_t prime = (seq_t)last_checked;

        /* digit sum of the prime */
        seq_t dsum = digit_sum(prime);

        /* concatenation of prime and its digit sum */
        seq_t concat = concat_parts((idx_t)prime, (idx_t)dsum);

        /* check if the concatenation is prime */
        if (is_prime_big(concat)) {
            /* grow cache if necessary */
            if (length == capacity) {
                size_t old_cap = capacity;
                capacity = capacity ? capacity << 1 : 16;
                seq_t *tmp = seq_realloc(terms, old_cap, capacity);
                terms = tmp;
            }
            terms[length++] = prime;
        }
    }

    return terms[n - 1];
}

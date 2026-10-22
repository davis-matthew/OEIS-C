/*
 * A362431  E.g.f. satisfies A(x) = exp(x - x^4 * A(x)).
 * https://oeis.org/A362431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/4)} (-1)^k * (k+1)^(n-3*k-1) / (k!
 *            * (n-4*k)!)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 18, i128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Special case */
    if (n == 0)
        return (seq_t)1;

    /* Static cache of factorials */
    static seq_t *fact = NULL;
    static size_t fact_size = 0;

    /* Ensure factorials up to n are computed */
    if (fact_size == 0) {
        fact_size = 1;
        fact = (seq_t*)malloc(sizeof(seq_t));
        fact[0] = (seq_t)1;          /* 0! = 1 */
    }
    /* Grow factorial cache if necessary */
    if (fact_size <= (size_t)n) {
        size_t required = (size_t)n + 1;
        while (fact_size < required) {
            size_t new_size = fact_size ? fact_size * 2 : 1;
            if (new_size < required)
                new_size = required;
            seq_t *new_fact = (seq_t*)realloc(fact, new_size * sizeof(seq_t));
            if (!new_fact)
                return (seq_t)0; /* allocation failure */
            for (idx_t i = fact_size; i < new_size; ++i) {
                new_fact[i] = new_fact[i - 1] * (seq_t)i;
            }
            fact = new_fact;
            fact_size = new_size;
        }
    }

    seq_t sum = (seq_t)0;
    idx_t max_k = n / 4;
    for (idx_t k = 0; k <= max_k; ++k) {
        /* Compute factorial ratio: n! / (k! * (n-4k)!) */
        seq_t term = fact[n];
        term = term / fact[k];
        term = term / fact[n - 4 * k];

        /* Power term: (k+1)^(n-3*k-1) */
        idx_t exp = n - 3 * k - 1;
        seq_t pow_term = big_pow((seq_t)(k + 1), (seq_t)exp);

        term = term * pow_term;

        /* Sign flip for odd k */
        if (k & 1)
            term = -term;

        sum += term;
    }
    return sum;
}

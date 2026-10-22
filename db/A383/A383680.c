/*
 * A383680  The greater of two consecutive primes whose gap equals the
 *          difference between their digital sums.
 * https://oeis.org/A383680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A383680: a(n) = greater of consecutive primes p_k < p_{k+1}
   such that p_{k+1} - p_k = digit_sum(p_{k+1}) - digit_sum(p_k) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/digit_sum.h"

/* Return the n-th term of sequence A383680.
   The sequence is built incrementally and memoised.
   This implementation works for any seq_t and idx_t types
   provided by the build system. */
seq_t a(idx_t n)
{
    /* memoised list of terms */
    static seq_t *terms      = NULL;
    static size_t terms_cap  = 0;
    static size_t terms_len  = 0;

    /* current consecutive prime pair */
    static uint64_t prev_prime   = 2;
    static uint64_t next_prime_v = 3;

    /* Generate terms until we have at least n of them. */
    while (terms_len < n) {
        seq_t diff   = (seq_t)(next_prime_v - prev_prime);
        seq_t ds_prev = digit_sum((seq_t)prev_prime);
        seq_t ds_next = digit_sum((seq_t)next_prime_v);

        if (diff == ds_next - ds_prev) {
            /* Ensure capacity of the term array. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Out of memory – terminate. */
                    abort();
                }
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)next_prime_v;
        }

        /* Advance to the next consecutive prime pair. */
        prev_prime = next_prime_v;
        next_prime_v = next_prime(prev_prime + 1);
    }

    return terms[n - 1];
}

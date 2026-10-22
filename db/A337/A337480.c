/*
 * A337480  Numbers k such that exactly one of 6*k + 5 and 12*k + 5 is prime.
 * https://oeis.org/A337480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 3162, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A337480: k such that exactly one of 6*k+5 and 12*k+5 is prime.
   Computation is done on demand with memoisation. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"  /* bool is_prime_small(uint64_t) */

/* static storage for already computed terms */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static idx_t  terms_len = 0;   /* number of terms stored */
static uint64_t last_checked = 0;  /* largest k examined so far */

static void ensure_capacity(size_t needed) {
    if (terms_cap < needed) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 8;
        if (new_cap < needed)
            new_cap = needed;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0)  /* offset is 1; behaviour for n==0 not required */
        return (seq_t)0;

    if (n > terms_len) {
        /* Generate additional terms until we have at least n of them */
        while (terms_len < n) {
            /* Find next k that satisfies the condition */
            uint64_t k = last_checked + 1;
            while (true) {
                uint64_t p1 = 6 * k + 5;
                uint64_t p2 = 12 * k + 5;
                bool is_p1 = is_prime_small(p1);
                bool is_p2 = is_prime_small(p2);
                if (is_p1 ^ is_p2) {  /* exactly one is prime */
                    /* store it */
                    ensure_capacity(terms_len + 1);
                    terms[terms_len++] = (seq_t)k;
                    last_checked = k;
                    break;
                }
                ++k;
            }
        }
    }
    return terms[n - 1];
}

/*
 * A351914  Numbers m such that the average of the prime numbers up to m is
 *          greater than or equal to m/2.
 * https://oeis.org/A351914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A351914: Numbers m such that 2·(sum of primes ≤ m) ≥ m·π(m) */
/*   where π(m) = number of primes ≤ m */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    /* static state while searching */
    static idx_t m = 2;              /* current integer to test */
    static seq_t sum_p = 0;          /* sum of primes ≤ m */
    static idx_t pi = 0;             /* number of primes ≤ m */
    static bool finished = false;    /* no further terms after 19 */

    if (n == 0) return 0;   /* defined only for n ≥ 1 */

    /* Allocate terms array on first use */
    if (terms == NULL) {
        terms_cap = 8;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) return 0;   /* allocation failed */
    }

    /* Generate terms until we have at least n terms or know there are none left */
    while (terms_len < n && !finished) {
        /* Find the next qualifying m */
        for (;;) {
            /* Test current m for primality */
            if (is_prime(m)) {
                ++pi;
                sum_p += (seq_t)m;
            }

            if (pi > 0) {
                seq_t left  = (seq_t)2 * sum_p;
                seq_t right = (seq_t)m * (seq_t)pi;
                if (left >= right) {
                    /* Store found term, expand array if necessary */
                    if (terms_len == terms_cap) {
                        terms_cap *= 2;
                        seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                        if (!tmp) return 0;   /* allocation failed */
                        terms = tmp;
                    }
                    terms[terms_len++] = (seq_t)m;
                    /* advance m for next search */
                    ++m;
                    break;
                }
            }

            /* After 19 no more terms exist */
            if (m > 19) {
                finished = true;
                break;
            }
            ++m;
        }
    }

    return (terms_len >= n) ? terms[n - 1] : 0;
}

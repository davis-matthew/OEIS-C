/*
 * A399286  Composite numbers whose number of digits and the number of digits
 *          in their largest prime factor are the same.
 * https://oeis.org/A399286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = integer enumerating composite numbers x such that
 *            digits(x) == digits(largest prime divisor of x). This
 *            implementation generates the sequence on demand, storing a cache
 *            of the first N terms and extending it until the requested index
 *            is reached
 * Range:     u8 n <= 43, u16 n <= 6173, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, digit_count, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/largest_prime_factor.h"
#include "../utils/digit_count.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* Lazy init of cache */
    static seq_t *terms  = NULL;     /* array of terms */
    static size_t term_cnt = 0;      /* number of terms stored */
    static size_t term_cap = 0;      /* current allocation size */
    static uint64_t cur = 4;         /* next number to test for condition */

    if (terms == NULL) {
        term_cap = 1024;
        terms  = (seq_t *)malloc(term_cap * sizeof(seq_t));
        if (!terms) return 0; /* allocation failed */
    }

    /* Generate terms until we have at least n of them */
    while (term_cnt < (size_t)n) {
        uint64_t x = cur;
        uint64_t lpf = largest_prime_factor(x);
        /* composite test: lpf != x */
        if (lpf != x) {
            if (digit_count(x) == digit_count(lpf)) {
                /* add term to cache */
                append_candidate(&terms, &term_cnt, &term_cap, (seq_t)x);
            }
        }
        cur++; /* next candidate number */
    }

    return terms[(size_t)n - 1];
}

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
 * Validated: standard, pass, 77 terms through n = 8130 (2026-11-03)
 * Helpers:   largest_prime_factor, mem, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <math.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/num_digits0.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* Lazy init of cache */
    static seq_t *terms  = NULL;     /* array of terms */
    static size_t term_cnt = 0;      /* number of terms stored */
    static size_t term_cap = 0;      /* current allocation size */
    static uint64_t cur = 4;         /* next number to test for condition */

    if (terms == NULL) {
        term_cap = 1024;
        terms  = mem_new(seq_t, term_cap);
        if (!terms) return 0; /* allocation failed */
    }

    /* Generate terms until we have at least n of them */
    while (term_cnt < (size_t)n) {
        uint64_t x = cur;
        uint64_t lpf = largest_prime_factor(x);
        /* composite test: lpf != x */
        if (lpf != x) {
            if (num_digits0(x) == num_digits0(lpf)) {
                /* add term to cache */
                append_candidate(&terms, &term_cnt, &term_cap, (seq_t)x);
            }
        }
        cur++; /* next candidate number */
    }

    return terms[(size_t)n - 1];
}

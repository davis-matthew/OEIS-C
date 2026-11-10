/*
 * A275608  Numbers that divide no nonzero terms of A003422.
 * https://oeis.org/A275608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that no 0!+1!+...+(m-1)! (m>=1) is
 *            divisible by k; found by sequential search, testing each k for m
 *            <= k
 * Range:     u8 n <= 206, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 8822 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Returns 1 if k divides some left factorial !m with m >= 1. */
static int A275608_divides_some(uint64_t k)
{
    uint64_t sum = 0, fact = 1 % k;
    for (uint64_t m = 1; m <= k; m++) {
        sum = (sum + fact) % k;
        if (sum == 0) return 1;
        fact = (fact * m) % k;
        if (fact == 0) return 0; /* sum stays constant from here on */
    }
    return 0;
}

static uint64_t *A275608_terms = 0;
static uint64_t A275608_count = 0, A275608_cap = 0, A275608_next = 1;

seq_t a(idx_t n)
{
    while (A275608_count < (uint64_t)n) {
        uint64_t k = A275608_next++;
        if (!A275608_divides_some(k)) {
            if (A275608_count == A275608_cap) {
                A275608_cap = A275608_cap ? A275608_cap * 2 : 1024;
                A275608_terms = (uint64_t *)realloc(A275608_terms, A275608_cap * sizeof(uint64_t));
            }
            A275608_terms[A275608_count++] = k;
        }
    }
    return (seq_t)A275608_terms[n - 1];
}

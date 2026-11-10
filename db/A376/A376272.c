/*
 * A376272  Elated numbers: numbers whose trajectory under iteration of the
 *          A376270 map includes 1.
 * https://oeis.org/A376272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k >= 1 whose trajectory under A376270 (k ->
 *            leading_digit(k)*sum of squares of digits(k)) reaches 1; found
 *            by search with Floyd cycle detection
 * Range:     u8 n <= 23, u16 n <= 4996, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9536 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A376272_f(uint64_t n)
{
    uint64_t sum = 0, t = n;
    while (t > 0) {
        uint64_t d = t % 10;
        sum += d * d;
        t /= 10;
    }
    t = n;
    while (t >= 10) t /= 10;
    return t * sum;
}

static int A376272_elated(uint64_t n)
{
    uint64_t tort = n, hare = n;
    for (;;) {
        tort = A376272_f(tort);
        hare = A376272_f(A376272_f(hare));
        if (tort == hare) break;
    }
    return tort == 1;
}

static uint64_t *A376272_terms = 0;
static size_t A376272_count = 0, A376272_cap = 0;
static uint64_t A376272_next = 1;

seq_t a(idx_t n)
{
    while (A376272_count < (size_t)n) {
        if (A376272_elated(A376272_next)) {
            if (A376272_count == A376272_cap) {
                size_t nc = A376272_cap ? A376272_cap * 2 : 1024;
                A376272_terms = (uint64_t *)realloc(A376272_terms, nc * sizeof(uint64_t));
                A376272_cap = nc;
            }
            A376272_terms[A376272_count++] = A376272_next;
        }
        A376272_next++;
    }
    return (seq_t)A376272_terms[(size_t)n - 1];
}

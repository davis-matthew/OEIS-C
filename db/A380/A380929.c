/*
 * A380929  Numbers k such that A380845(k) > 2*k.
 * https://oeis.org/A380929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th k such that Sum_{d|k, popcount(d)=popcount(k)} d >
 *            2*k; found by incremental search with cached terms
 * Range:     u8 n <= 7, u16 n <= 2299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9949 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static int A380929_pc(uint64_t x)
{
    int c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
}

static bool A380929_is_term(uint64_t k)
{
    int pc = A380929_pc(k);
    uint64_t sum = 0;
    for (uint64_t i = 1; i * i <= k; ++i) {
        if (k % i == 0) {
            uint64_t j = k / i;
            if (A380929_pc(i) == pc) sum += i;
            if (j != i && A380929_pc(j) == pc) sum += j;
        }
    }
    return sum > 2 * k;
}

static uint64_t *A380929_terms = 0;
static uint64_t A380929_count = 0;
static uint64_t A380929_cap = 0;
static uint64_t A380929_next = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A380929_count < (uint64_t)n) {
        uint64_t k = A380929_next++;
        if (A380929_is_term(k)) {
            if (A380929_count == A380929_cap) {
                A380929_cap = A380929_cap ? A380929_cap * 2 : 1024;
                A380929_terms = (uint64_t *)realloc(A380929_terms,
                                    A380929_cap * sizeof(uint64_t));
            }
            A380929_terms[A380929_count++] = k;
        }
    }
    return (seq_t)A380929_terms[n - 1];
}

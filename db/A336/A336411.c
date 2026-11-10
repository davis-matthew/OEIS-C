/*
 * A336411  a(n) = A029707(n+2) - 3 for n >= 1.
 * https://oeis.org/A336411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A029707(n+2) - 3, where A029707(k) is the index m such
 *            that prime(m), prime(m+1) are twin primes; computed by a
 *            doubling sieve
 * Range:     u8 n <= 50, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static uint64_t *A336411_tw = NULL;
static size_t A336411_cnt = 0;
static uint64_t A336411_limit = 0;

static void A336411_build(uint64_t limit)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)limit + 1, 1);
    if (!comp) exit(EXIT_FAILURE);
    for (uint64_t i = 2; i * i <= limit; ++i)
        if (!comp[i])
            for (uint64_t j = i * i; j <= limit; j += i) comp[j] = 1;
    free(A336411_tw);
    A336411_tw = NULL;
    size_t cap = 0;
    A336411_cnt = 0;
    uint64_t idx = 0, prev = 0;
    for (uint64_t p = 2; p <= limit; ++p) {
        if (comp[p]) continue;
        ++idx;
        if (prev && p == prev + 2) {
            if (A336411_cnt == cap) {
                cap = cap ? cap * 2 : 64;
                A336411_tw = (uint64_t *)realloc(A336411_tw, cap * sizeof(uint64_t));
                if (!A336411_tw) exit(EXIT_FAILURE);
            }
            A336411_tw[A336411_cnt++] = idx - 1; /* index of the first prime */
        }
        prev = p;
    }
    free(comp);
    A336411_limit = limit;
}

seq_t a(idx_t n)
{
    size_t need = (size_t)n + 2;
    if (A336411_limit == 0) A336411_build(1u << 16);
    while (A336411_cnt < need) A336411_build(A336411_limit * 2);
    return (seq_t)(A336411_tw[need - 1] - 3);
}

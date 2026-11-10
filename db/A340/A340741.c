/*
 * A340741  Numbers k such that A340740(k) is prime.
 * https://oeis.org/A340741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k-th (in increasing order) index m with A340740(m) prime,
 *            where A340740(m) = Sum_{j=1..floor((m-1)/2), gcd(m,j)=1} (m mod
 *            j); found by a search
 * Range:     u8 n <= 45, u16 n <= 3998, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 81 terms through n = 4860 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static uint64_t *T_terms = 0;
static size_t T_cnt = 0, T_cap = 0;
static uint64_t T_next = 1;
static unsigned char *T_mark = 0;
static size_t T_markcap = 0;

static uint64_t A340740_val(uint64_t m)
{
    uint64_t limit = (m - 1) / 2;
    if (limit == 0) return 0;
    if (T_markcap < limit + 1) {
        free(T_mark);
        T_markcap = (size_t)(limit + 1) * 2;
        T_mark = (unsigned char *)malloc(T_markcap);
    }
    for (uint64_t j = 0; j <= limit; j++) T_mark[j] = 0;
    uint64_t x = m;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            while (x % p == 0) x /= p;
            for (uint64_t j = p; j <= limit; j += p) T_mark[j] = 1;
        }
    }
    if (x > 1) {
        for (uint64_t j = x; j <= limit; j += x) T_mark[j] = 1;
    }
    uint64_t total = 0;
    for (uint64_t j = 1; j <= limit; j++)
        if (!T_mark[j]) total += m % j;
    return total;
}

seq_t a(idx_t n)
{
    while (T_cnt < (size_t)n) {
        uint64_t k = T_next++;
        uint64_t v = A340740_val(k);
        if (v >= 2 && is_prime_u64(v)) {
            if (T_cnt == T_cap) {
                T_cap = T_cap ? T_cap * 2 : 1024;
                T_terms = (uint64_t *)realloc(T_terms, T_cap * sizeof(uint64_t));
            }
            T_terms[T_cnt++] = k;
        }
    }
    return (seq_t)T_terms[(size_t)n - 1];
}

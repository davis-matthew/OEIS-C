/*
 * A350577  Prime numbers in A036991.
 * https://oeis.org/A350577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th prime p whose binary expansion, read from the least
 *            significant bit, never has more 0's than 1's (search over odd
 *            numbers, primality test)
 * Range:     u8 n <= 33, u16 n <= 2727, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 72 terms through n = 9825 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t *A350577_terms = NULL;
static size_t A350577_cnt = 0;
static size_t A350577_cap = 0;
static uint64_t A350577_next = 3;

static bool A350577_valid(uint64_t x)
{
    uint64_t bal = 0;
    while (x > 0) {
        if (x & 1)
            bal++;
        else {
            if (bal == 0)
                return false;
            bal--;
        }
        x >>= 1;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    while (A350577_cnt < (size_t)n) {
        uint64_t k = A350577_next;
        A350577_next += 2;
        if (!A350577_valid(k))
            continue;
        if (!is_prime_u64(k))
            continue;
        if (A350577_cnt == A350577_cap) {
            size_t nc = A350577_cap ? A350577_cap * 2 : 1024;
            uint64_t *p = (uint64_t *)realloc(A350577_terms, nc * sizeof(uint64_t));
            if (!p)
                return (seq_t)0;
            A350577_terms = p;
            A350577_cap = nc;
        }
        A350577_terms[A350577_cnt++] = k;
    }
    return (seq_t)A350577_terms[n - 1];
}

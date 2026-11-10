/*
 * A337373  Numbers k for which A003961(k) > 3*k.
 * https://oeis.org/A337373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A003961(k) > 3*k; found by search (A003961 is
 *            completely multiplicative, prime(i) -> prime(i+1))
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9079 (2026-11-08)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

static uint64_t A003961_u64(uint64_t m) {
    uint64_t result = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= q;
            }
        }
    }
    if (m > 1) {
        result *= next_prime(m);
    }
    return result;
}

static uint64_t *A337373_terms = NULL;
static uint64_t A337373_cnt = 0;
static uint64_t A337373_cap = 0;
static uint64_t A337373_last = 0;

seq_t a(idx_t n) {
    while (A337373_cnt < (uint64_t)n) {
        uint64_t k = ++A337373_last;
        if (A003961_u64(k) > 3 * k) {
            if (A337373_cnt == A337373_cap) {
                A337373_cap = A337373_cap ? A337373_cap * 2 : 1024;
                A337373_terms = (uint64_t *)realloc(A337373_terms, A337373_cap * sizeof(uint64_t));
            }
            A337373_terms[A337373_cnt++] = k;
        }
    }
    return (seq_t)A337373_terms[n - 1];
}

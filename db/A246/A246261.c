/*
 * A246261  Numbers n such that A003961(n) is of the form 4k+1.
 * https://oeis.org/A246261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A003961(k) == 1 (mod 4); equivalently
 *            the number of prime factors p of k (with multiplicity) with
 *            nextprime(p) == 3 (mod 4) is even. Computed by incremental
 *            search with a cache
 * Range:     u8 n <= 124, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 92 terms through n = 8464 (2026-11-07)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static bool A246261_good(uint64_t m) {
    unsigned cnt = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            while (m % p == 0) {
                m /= p;
                if ((q & 3) == 3) cnt++;
            }
        }
    }
    if (m > 1) {
        if ((next_prime(m) & 3) == 3) cnt++;
    }
    return (cnt & 1) == 0;
}

static uint64_t *A246261_terms = NULL;
static size_t A246261_count = 0;
static size_t A246261_cap = 0;
static uint64_t A246261_next = 1;

seq_t a(idx_t n) {
    while (A246261_count < (size_t)n) {
        if (A246261_good(A246261_next)) {
            if (A246261_count == A246261_cap) {
                size_t nc = A246261_cap ? A246261_cap * 2 : 1024;
                uint64_t *t = (uint64_t *)realloc(A246261_terms, nc * sizeof(uint64_t));
                if (!t) return 0;
                A246261_terms = t;
                A246261_cap = nc;
            }
            A246261_terms[A246261_count++] = A246261_next;
        }
        A246261_next++;
    }
    return (seq_t)A246261_terms[n - 1];
}

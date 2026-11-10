/*
 * A337344  Odd numbers k such that A064989(sigma(k)) >= k.
 * https://oeis.org/A337344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd k such that A064989(sigma(k)) >= k; computed by
 *            a incremental search over odd k with memoized terms
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-08)
 * Helpers:   factor_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_u64.h"

static uint64_t A337344_prev_prime(uint64_t n)
{
    uint64_t p = n - 1;
    while (!is_prime_u64(p)) --p;
    return p;
}

static uint64_t A337344_sigma(uint64_t k)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(k, pr, ex, &cnt);
    uint64_t s = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t term = 1, pw = 1;
        for (uint64_t j = 0; j < ex[i]; ++j) {
            pw *= pr[i];
            term += pw;
        }
        s *= term;
    }
    return s;
}

static uint64_t A337344_A064989(uint64_t m)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    uint64_t res = 1;
    if (m < 3) return 1;
    factor_u64(m, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] == 2) continue;
        uint64_t b = A337344_prev_prime(pr[i]);
        for (uint64_t j = 0; j < ex[i]; ++j) res *= b;
    }
    return res;
}

static uint64_t *A337344_terms = NULL;
static size_t A337344_count = 0, A337344_cap = 0;
static uint64_t A337344_next = 1;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A337344_count < (size_t)n) {
        uint64_t k = A337344_next;
        A337344_next += 2;
        if (A337344_A064989(A337344_sigma(k)) >= k) {
            if (A337344_count == A337344_cap) {
                A337344_cap = A337344_cap ? A337344_cap * 2 : 1024;
                A337344_terms = (uint64_t *)realloc(A337344_terms, A337344_cap * sizeof(uint64_t));
            }
            A337344_terms[A337344_count++] = k;
        }
    }
    return (seq_t)A337344_terms[n - 1];
}

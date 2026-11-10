/*
 * A337185  Composite numbers k such that A337183(k) is prime.
 * https://oeis.org/A337185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th composite k such that A337183(k) is prime
 *            (A337183(k) = factors of k read in base sum of factors);
 *            computed by a search with memoization
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 864, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9895 (2026-11-08)
 * Helpers:   is_prime, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_seq.h"

static seq_t A337185_val(uint64_t k)
{
    uint64_t pf[64];
    size_t cnt = 0;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; ++p) {
        while (m % p == 0) {
            pf[cnt++] = p;
            m /= p;
        }
    }
    if (m > 1) pf[cnt++] = m;
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) sum += (seq_t)pf[i];
    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i) res = res * sum + (seq_t)pf[i];
    return res;
}

static uint64_t *A337185_terms = NULL;
static size_t A337185_count = 0;
static size_t A337185_cap = 0;
static uint64_t A337185_next = 4;

seq_t a(idx_t n)
{
    while (A337185_count < (size_t)n) {
        uint64_t k = A337185_next++;
        if (is_prime((idx_t)k)) continue;
        seq_t v = A337185_val(k);
        if (is_prime_seq(v)) {
            if (A337185_count == A337185_cap) {
                A337185_cap = A337185_cap ? A337185_cap * 2 : 1024;
                A337185_terms = (uint64_t *)realloc(A337185_terms, A337185_cap * sizeof(uint64_t));
            }
            A337185_terms[A337185_count++] = k;
        }
    }
    return (seq_t)A337185_terms[(size_t)n - 1];
}

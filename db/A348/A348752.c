/*
 * A348752  Numbers k for which A348750(k) > k.
 * https://oeis.org/A348752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A348750(k) > k, where A348750(k) =
 *            A064989(A064989(sigma(A003961(A003961(k))))); found by a
 *            sequential search over k
 * Range:     u8 n <= 11, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-10)
 * Helpers:   factor_u64, is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

static inline idx_t A348750_prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p) {
        if (is_prime(p)) return p;
    }
}

static uint64_t A348750_shift_down_once(uint64_t m)
{
    uint64_t primes[32], exps[32];
    size_t count;
    factor_u64(m, primes, exps, &count);
    uint64_t res = 1;
    for (size_t i = 0; i < count; ++i) {
        uint64_t prime = primes[i];
        if (prime == 2) continue;
        uint64_t prev = A348750_prev_prime(prime);
        unsigned int exp = (unsigned int)exps[i];
        for (unsigned int k = 0; k < exp; ++k) res *= prev;
    }
    return res;
}

static seq_t A348750_a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t primes_n[32], exps_n[32];
    size_t cnt_n;
    factor_u64((uint64_t)n, primes_n, exps_n, &cnt_n);
    seq_t result = 1;
    for (size_t i = 0; i < cnt_n; ++i) {
        uint64_t p = primes_n[i];
        unsigned int e = (unsigned int)exps_n[i];
        uint64_t q1 = next_prime(p);
        uint64_t q = next_prime(q1);
        __int128 pw = 1;
        for (unsigned int j = 0; j < e + 1; ++j) pw *= q;
        uint64_t val = (uint64_t)((pw - 1) / (q - 1));
        uint64_t t1 = A348750_shift_down_once(val);
        uint64_t t2 = A348750_shift_down_once(t1);
        result *= (seq_t)t2;
    }
    return result;
}

static idx_t *A348752_terms = NULL;
static size_t A348752_cnt = 0;
static size_t A348752_cap = 0;
static idx_t A348752_last = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A348752_cnt < (size_t)n) {
        A348752_last++;
        if (A348750_a(A348752_last) > (seq_t)A348752_last) {
            if (A348752_cnt == A348752_cap) {
                size_t nc = A348752_cap ? A348752_cap * 2 : 256;
                A348752_terms = (idx_t *)realloc(A348752_terms, nc * sizeof(idx_t));
                A348752_cap = nc;
            }
            A348752_terms[A348752_cnt++] = A348752_last;
        }
    }
    return (seq_t)A348752_terms[n - 1];
}

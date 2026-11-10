/*
 * A318957  Numbers k such that A194113(k) is a prime.
 * https://oeis.org/A318957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) = n-th k such that A194113(k) = Sum_{j=1..k}
 *            floor(j*sqrt(10)) is prime
 * Range:     u8 n <= 20, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-08)
 * Helpers:   is_prime_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"

static uint64_t *S318957_terms = 0;
static size_t S318957_cnt = 0, S318957_cap = 0;
static uint64_t S318957_k = 0, S318957_sum = 0;

seq_t a(idx_t n)
{
    while (S318957_cnt < (size_t)n) {
        ++S318957_k;
        S318957_sum += isqrt_u64(10ULL * S318957_k * S318957_k);
        if (is_prime_u64(S318957_sum)) {
            if (S318957_cnt == S318957_cap) {
                S318957_cap = S318957_cap ? S318957_cap * 2 : 64;
                S318957_terms = (uint64_t *)realloc(S318957_terms, S318957_cap * sizeof(uint64_t));
            }
            S318957_terms[S318957_cnt++] = S318957_k;
        }
    }
    return (seq_t)S318957_terms[n - 1];
}

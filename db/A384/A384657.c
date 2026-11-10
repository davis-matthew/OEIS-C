/*
 * A384657  Numbers k such that A384655(k) > k.
 * https://oeis.org/A384657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A384655(k) > k, where A384655(k) = Sum_{d|k}
 *            A051903(d)*phi(k/d); computed by a sequential search with cached
 *            terms
 * Range:     u8 n <= 18, u16 n <= 5310, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9497 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static void A384657_rec(int idx, const uint64_t *p, const uint64_t *e,
                        int pcnt, uint64_t max_e, uint64_t phi_acc, uint64_t *acc)
{
    if (idx == pcnt) {
        *acc += max_e * phi_acc;
        return;
    }
    uint64_t prime = p[idx];
    uint64_t ex = e[idx];
    uint64_t powk[65];
    powk[0] = 1;
    for (uint64_t k = 1; k <= ex; ++k)
        powk[k] = powk[k - 1] * prime;
    for (uint64_t d = 0; d <= ex; ++d) {
        uint64_t rem = ex - d;
        uint64_t ph = (rem == 0) ? 1 : (prime - 1) * powk[rem - 1];
        A384657_rec(idx + 1, p, e, pcnt, d > max_e ? d : max_e, phi_acc * ph, acc);
    }
}

static uint64_t A384657_f(uint64_t n)
{
    uint64_t primes[20];
    uint64_t exps[20];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);
    uint64_t sum = 0;
    A384657_rec(0, primes, exps, (int)pcnt, 0, 1, &sum);
    return sum;
}

static uint64_t *A384657_terms = NULL;
static size_t A384657_cnt = 0;
static size_t A384657_cap = 0;
static uint64_t A384657_next = 1;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A384657_cnt < (size_t)n) {
        uint64_t k = A384657_next++;
        if (k < 4) continue;
        if (A384657_f(k) > k) {
            if (A384657_cnt == A384657_cap) {
                size_t nc = A384657_cap ? A384657_cap * 2 : 1024;
                A384657_terms = (uint64_t *)realloc(A384657_terms, nc * sizeof(uint64_t));
                A384657_cap = nc;
            }
            A384657_terms[A384657_cnt++] = k;
        }
    }
    return (seq_t)A384657_terms[n - 1];
}
